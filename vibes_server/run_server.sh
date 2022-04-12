#!/bin/bash
podman run --rm -it --name=vibes-site -v ./static:/static -v ./templates:/templates -p 1883:1883 -p 8080:8080 vibes-site
