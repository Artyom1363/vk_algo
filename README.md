# vk_algo

$ docker build . -f Dockerfile -t image_gdb
$ docker run --name=algo --cap-add=SYS_PTRACE --security-opt seccomp=unconfined -it -v /Users/artem/Documents/BMSTU/vk_algo:/app image_gdb:latest