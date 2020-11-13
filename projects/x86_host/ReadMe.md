# x86 host
So the x86 host is a nice way of saying that you are using a regular computer with a regular CPU as host. This means that manny of the considerations that usually have to be made when using e.g. a Raspberry Pi, do not have to be concidered. Moreover, this decision means that you decided to go with the slightly more complex route of using gateways that send the data to your host. The host collects the information of 1..* gateways, stores this in a central database and presents this data in a nice dashboard. The entire networking and infrastructure is build on Docker as this is a simple way of doing things. We use PostgREST as a API to DB interface, we use Postgres as central DB and use Grafana as dashboarding tool. To comply with best practises we produce a virtual network within Docker to deploy these containers. We assume a Linux setup for the Docker deployment script.

## Docker and her containers
First things first, Docker is awesome! Docker uses containers to deploy applications "in the field". The advantage (amongs many other) of using Docker, is that Docker deploys the applications by deploying images (like recipies) in containers. Containers are stand-alone applications that contains programs and configurations, all in a single box. Docker does this in a very efficient way costing very little computional overhead. To add Docker is very well supported by all relevant software companies and by the Open-Source community. If you really want to understand all the ins and outs on Docker, I strongly recommend taking the Docker Mastery course that can be found on [Udemy](https://www.udemy.com/course/docker-mastery/). 
![Docker-Logo](../../doc/Moby-logo.png)

In our case we want to use Docker to:
1. Setup our own virtual-network for our applications
2. Spin up PostgREST to interface with your DB over API.
2. Spin up a Postgres Database
3. Spin up Grafana a Dashboarding tool. 

Keep in mind that namespace within in the network is identical to name of the container. So if I would be connecting my DB-IDE tool to the database, I would use in this case `localhost:5432`, but when I want to connect Grafana to the database (which are in the same virtual network), I would use `db:5432` because the container is named `db`. 

Another interesting note is that containers do not have to be persistent, which means that if you shut down your host, you would lose the data in your database and dashboarding utility. We have added volumes to make the database and dashboard persistant. If you restart your host, you only have to start the exisitng containers with e.g. `docker start db`, and you should be golden. 