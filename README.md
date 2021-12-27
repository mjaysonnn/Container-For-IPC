# Container-For-IPC

## Image name in docker hub

smj8612/mj-ipc:1.3

## For interacting in init-container

docker run -d -t mj-ipc:1.3
docker ps
docker exec -it c882d03bd955 bash

## source for making an image (when you have to manipulate init container)

cd SourceCode
docker build --tag <image-name-you-want> .

## running an init container for verification & check log to see if it was correct

cd SourceCode
kubectl apply -f ipc-as-init-container.yml
kubectl get pods
kubectl logs <pod-name>

## If you want to connect with container to do test

docker exec -it c882d03bd955 bash
