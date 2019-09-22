#! /bin/sh
docker run --privileged --rm -v `pwd`:/build_root centos6.6-gcc5.4 /build_root/docker_build.sh `id -u` $1
# cat ../.git/`git symbolic-ref HEAD` > bin-centos/git-head
# git status --porcelain > bin-centos/git-status
cat ../.git/`git symbolic-ref HEAD` > bin/git-head
git status --porcelain > bin/git-status
