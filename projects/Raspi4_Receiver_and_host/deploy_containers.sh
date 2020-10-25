#!/bin/bash

docker network create -d bridge garden_net

docker run -d \
        --name db \
        --net garden_net \
        -p 5432:5432 \
        -e POSTGRES_PASSWORD="overlord"\
        -e POSTGRES_DB="data"\
        -e PGDATA=/var/lib/postgresql/data/pgdata \
        postgres

docker run -d \
        --name=grafana \
        --net garden_net \
        -p 80:3000 \
        -v grafana-storage:/var/lib/grafana \
        grafana/grafana

