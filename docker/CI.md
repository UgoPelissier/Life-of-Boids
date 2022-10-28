# Procedure for CI

## Building a docker image

Run the following command from the root directory.

`docker build -t boids-image -f docker/Dockerfile .`

## Optional: Running the docker image locally (just to make sure if we can build the project inside docker container)

```
# Assuming the current directory is the project's root directory
docker run -it --rm -v /$PWD:/usr/project/ --name boids boids-image:latest
```

## Start a runner (These commands are specific to our project)

### For Windows
```
# To configure the runner
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

# To run the gitlab-runner
winpty docker run -d --name gitlab-runner --restart always \
	-v /$PWD/gitlab_runner:/etc/gitlab-runner \
	-v //var/run/docker.sock:/var/run/docker.sock \
	gitlab/gitlab-runner
```

### For Unix (Linux or Mac)
```
# To configure the runner
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

# To run the gitlab-runner
docker run -d --name gitlab-runner --restart always \
	-v $PWD/gitlab_runner:/etc/gitlab-runner \
	-v /var/run/docker.sock:/var/run/docker.sock \
	gitlab/gitlab-runner
```

**Note:** The configuration from above will generate `gitlab-runner` folder where you run the command.

## Some small changes needed to use the locally built image
- The previous image, Pascal shared with us, was from the dockerhub which is a public repository and anyone can pull it.
- Now, we are using our custom docker image which resides on our local machine.
- The default nature of gitlab-ci is to pull the image from dockerhub without checking for local images.
- For gitlab-runner to find our local image, we have to update `gitlab-runner/config.taml`
- Add the following to change the default behaviour of pulling docker images
```
[runners.docker]
pull_policy = "if-not-present"
```

## Steps to run the pipeline inside Gitlab

- Specify the image name `boids-image` in `.gitlab-ci.yaml` file. (The image must be present in the runner)
- Either execute build.sh or copy those commands in `.gitlab-ci.yaml` file.
- Either execute test.sh or copy those commands in `.gitlab-ci.yaml` file.


## Done
- So, every time we push, GitLab will run the commands inside `boids-image`.
- We then check the logs of the pipeline to make sure that the build was successful and the program passed all the tests.
- One thing to add here, Since I've built this image from scratch, not all the libraries were installed. So, to build the missing ones
  we pass the flag `--build=missing` to the `conan install`. This will download all the missing libraries inside the docker. This process may take longer depending on the internet speed, of course.
- To avoid this waiting, for the time being, I have disabled the CICD process in our project. [Click here](https://docs.gitlab.com/ee/ci/enable_or_disable_ci.html) to enable/disable CICD on GitLab.

