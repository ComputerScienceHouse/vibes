FROM golang as builder

WORKDIR /build
COPY go.mod go.sum *.go . 
#RUN go mod tidy
RUN CGO_ENABLED=0 GOOS=linux go build -a -o vibes .

FROM alpine:latest

COPY --from=builder /build/vibes .
COPY ./templates ./templates
COPY ./static ./static
ENTRYPOINT ["./vibes"]
