docker build --progress plain -f DockerFile-Gentoo-x64-base -t sumeetchhetri/gentoo-stage3-amd64:1.0 .
docker push sumeetchhetri/gentoo-stage3-amd64:1.0

docker build -f DockerFile-ArchLinux-x64-ffead-cpp --tag sumeetchhetri/ffead-cpp:ffead-cpp-archlinux .
docker build -f DockerFile-UbuntuBionic-x64-ffead-cpp --tag sumeetchhetri/ffead-cpp:ffead-cpp-ubuntubionic .
docker build -f DockerFile-OpenSuseTumbleweed-x64-ffead-cpp --tag sumeetchhetri/ffead-cpp:ffead-cpp-opensusetw .
docker build -f DockerFile-Centos7-x64-ffead-cpp --tag sumeetchhetri/ffead-cpp:ffead-cpp-centos7 .
docker build -f DockerFile-Gentoo-x64-ffead-cpp --tag sumeetchhetri/ffead-cpp:ffead-cpp-gentoo .