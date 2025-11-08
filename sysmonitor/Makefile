# Makefile for monitor (local build) and Docker image (uses provided Dockerfile)

# Compiler & flags
CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra
LDFLAGS := -lncurses

# Files / names
SRC := monitor.cpp
BIN := monitor
DOCKER_IMAGE := simple-sysmon:latest
DOCKER_CONTAINER := simple-sysmon-run

# Default target
.PHONY: all
all: build

# Build local binary
.PHONY: build
build: $(BIN)

$(BIN): $(SRC)
	@echo "-> Building local binary: $(BIN)"
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

# Run local binary (passes through PROC_ROOT if set in environment)
.PHONY: run
run: build
	@echo "-> Running ./$(BIN) (press q to quit)"
	./$(BIN)

# Build docker image (uses Dockerfile in current dir)
.PHONY: docker-build
docker-build:
	@echo "-> Building Docker image: $(DOCKER_IMAGE)"
	docker build -t $(DOCKER_IMAGE) .

# Run docker image in an isolated container (default; /proc inside container)
# You can pass extra docker args via DOCKER_ARGS, e.g. make docker-run DOCKER_ARGS="-e PROC_ROOT=/proc -v /tmp:/tmp"
.PHONY: docker-run
docker-run: docker-build
	@echo "-> Running Docker container from image $(DOCKER_IMAGE)"
	docker run --rm -it --name $(DOCKER_CONTAINER) $(DOCKER_ARGS) $(DOCKER_IMAGE)

# Convenience: run container to monitor the host's /proc (needs --pid=host)
# This mounts the host /proc at /hostproc and sets PROC_ROOT=/hostproc inside the container.
# Note: --pid=host is required to see host PIDs correctly.
.PHONY: docker-run-host
docker-run-host: docker-build
	@echo "-> Running Docker container to monitor host /proc (needs permissions)"
	docker run --rm -it \
	  --name $(DOCKER_CONTAINER) \
	  --pid=host \
	  -v /proc:/hostproc:ro \
	  -e PROC_ROOT=/hostproc \
	  $(DOCKER_ARGS) \
	  $(DOCKER_IMAGE)

# Clean up build artifacts
.PHONY: clean
clean:
	@echo "-> Cleaning local build artifacts"
	-rm -f $(BIN)

# Remove the docker image (if you want)
.PHONY: docker-clean
docker-clean:
	@echo "-> Removing docker image: $(DOCKER_IMAGE)"
	-docker rmi $(DOCKER_IMAGE) || true

# Help text
.PHONY: help
help:
	@printf "\nUsage:\n\n"
	@printf "  make           -> build local binary (same as 'make build')\n"
	@printf "  make build     -> compile monitor binary (./monitor)\n"
	@printf "  make run       -> build then run local binary\n"
	@printf "  make docker-build -> build Docker image (uses Dockerfile)\n"
	@printf "  make docker-run   -> run the image in a container (default /proc inside container)\n"
	@printf "  make docker-run-host -> run container to view host /proc (uses --pid=host and mounts /proc)\n"
	@printf "  make clean     -> remove local binary\n"
	@printf "  make docker-clean -> remove built docker image\n"
	@printf "\nYou can pass extra docker args with DOCKER_ARGS, e.g.:\n\n"
	@printf "  make docker-run DOCKER_ARGS=\"-e PROC_ROOT=/proc -v /tmp:/tmp\"\n\n"

