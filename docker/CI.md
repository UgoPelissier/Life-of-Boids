# Procedure for CI

**Note**: The docker image we created and use for CI has been pushed to Docker Hub. Therefore, there is no need to build the image locally since the CI process will automatically pull the image from Docker Hub. However, if one wants to build it locally, we can use the following command from the root directory.

`docker pull ugopelissierminesparis/life-of-boids:latest`

## Optional: Running the docker image locally
Assuming the current directory is the project's root directory
```
docker run -it --rm -v /$PWD:/usr/project/ --name boids ugopelissierminesparis/life-of-boids
```

## Start a runner

These commands are specific to our project.

### Windows
To configure the runner
```
winpty docker run --rm -it \
	-v /$PWD/gitlab_runner:/etc/gitlab-runner \
	--name gitlab-runner \
	gitlab/gitlab-runner \
	register \
	--non-interactive \
	--executor "docker" \
	--docker-image alpine:3.9 \
	--url "https://git.sophia.mines-paristech.fr/" \
	--registration-token "GR1348941jqt1XzFRsc26p-mKxzsh" \
	--description "docker-runner" \
	--tag-list "docker,mytag" \
	--run-untagged \
	--locked="false"
```

To run the gitlab-runner
```
winpty docker run -d --name gitlab-runner --restart always \
	-v /$PWD/gitlab_runner:/etc/gitlab-runner \
	-v //var/run/docker.sock:/var/run/docker.sock \
	gitlab/gitlab-runner
```

### Unix (Linux or MacOS)
To configure the runner
```
docker run --rm -it \
	-v $PWD/gitlab_runner:/etc/gitlab-runner \
	--name gitlab-runner \
	gitlab/gitlab-runner \
	register \
	--non-interactive \
	--executor "docker" \
	--docker-image alpine:3.9 \
	--url "https://git.sophia.mines-paristech.fr/" \
	--registration-token "GR1348941jqt1XzFRsc26p-mKxzsh" \
	--description "docker-runner" \
	--tag-list "docker,mytag" \
	--run-untagged \
	--locked="false"
```

To run the gitlab-runner
```
docker run -d --name gitlab-runner --restart always \
	-v $PWD/gitlab_runner:/etc/gitlab-runner \
	-v /var/run/docker.sock:/var/run/docker.sock \
	gitlab/gitlab-runner
```

**Note:** The configuration from above will generate `gitlab-runner` folder where you run the command.

## Running the pipeline inside Gitlab

- Specify the image name `ugopelissierminesparis/life-of-boids` in `.gitlab-ci.yaml` file.


