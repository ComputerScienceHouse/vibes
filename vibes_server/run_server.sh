#!/bin/bash
podman run --rm -it --name=vibes-site -v ./static:/static -v ./templates:/templates -p 8080:8080 vibes-site
