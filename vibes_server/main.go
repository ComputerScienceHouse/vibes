package main

import (
	"fmt"

	mqtt "github.com/eclipse/paho.mqtt.golang"
	"github.com/gin-gonic/gin"
)

var flavors = map[string]string{
    "good": "Good Vibes",
    "bad": "Bad Vibes",
}

// Handle messages from subscribed topics
var messagePubHandler mqtt.MessageHandler = func(client mqtt.Client, msg mqtt.Message) {
	fmt.Printf("Received message: %s from topic: %s\n", msg.Payload(), msg.Topic())
}

var connectHandler mqtt.OnConnectHandler = func(client mqtt.Client) {
	fmt.Println("Connected\n")
}

var connectLostHandler mqtt.ConnectionLostHandler = func(client mqtt.Client, err error) {
	fmt.Printf("Connect lost: %v\n", err)
}

func sub(client mqtt.Client, topic string) {
	token := client.Subscribe(topic, 1, messagePubHandler)
	token.Wait()
	fmt.Printf("Subscribed to topic %s", topic)
}

func update_sign(client mqtt.Client, good_vibes int, bad_vibes int) {
    // TODO: Compare number of good vibes to number of bad vibes
    // Glow each sign based on that number
    total_vibe_checks := good_vibes + bad_vibes

    good_glow := 255 * (good_vibes/total_vibe_checks)
    bad_glow := 255 * (bad_vibes/total_vibe_checks)

    token := client.Publish("vibes/bad", 0, false, bad_glow)
    token.Wait()

    token = client.Publish("vibes/good", 0, false, good_glow)
    token.Wait()
}

func main() {

    var good_vibes = 0
    var bad_vibes = 0

	// MQTT setup (and a lot of it)
	var broker = "mqtt.csh.rit.edu"
	var port = 1883
	opts := mqtt.NewClientOptions()
	opts.AddBroker(fmt.Sprintf("tcp://%s:%d", broker, port))
	opts.SetClientID("go_mqtt_client")
	opts.SetDefaultPublishHandler(messagePubHandler)
	opts.OnConnect = connectHandler
	opts.OnConnectionLost = connectLostHandler
	client := mqtt.NewClient(opts)
	if token := client.Connect(); token.Wait() && token.Error() != nil {
		panic(token.Error())
	}

	// Subscribe to topics. This server don't need no subscriptions (yet).

	// Gin Setup
	r := gin.Default()
	r.SetTrustedProxies([]string{"0.0.0.0"})
	r.LoadHTMLGlob("/templates/*")
	r.Static("/static", "/static")

	// Route definitions

    // Homepage
	r.GET("/", func(c *gin.Context) {
		c.HTML(200, "home.tmpl", gin.H{
            "vibes_map" : flavors,
        })
	})

    // TODO: Login

    // Request to submit vibes
	r.POST("/vibes/:flavor", func(c *gin.Context) {
        flavor, exists := flavors[c.Param("flavor")]

        // TODO: Check if user has submitted vibes today.
        if exists {
           if flavor == flavors["good"] {
               good_vibes++
           } 
           if flavor == flavors["bad"] {
               bad_vibes++
           }
        } else {
            c.String(404, "Unknown Location.");
        }
        update_sign(client, good_vibes, bad_vibes)
	})

    // See current vibe stats
    r.GET("/stats/:response", func(c *gin.Context) {
        user_response := flavors[c.Param("response")] // get the user's response
        c.HTML(200, "stats.tmpl", gin.H{
            "user_response" : user_response,
        })
    })

    r.GET("/vibes/:flavor", func(c *gin.Context){
        flavor, _ := flavors[c.Param("flavor")]
        if flavor == flavors["good"] {
            c.JSON(200, gin.H{"good_vibes":good_vibes})
            return
        }
        if flavor == flavors["bad"] {
            c.JSON(200, gin.H{"bad_vibes":bad_vibes})
            return
        }
        if c.Param("flavor") == "all" {
            c.JSON(200, gin.H{"good_vibes":good_vibes, "bad_vibes":bad_vibes})
            return
        }
    })

  	r.Run()

	// chom
	client.Disconnect(250)
}
