# RealMedium

## THIS SAMPLE IS NOT READY! COME BACK LATER

This is project created in summer school Yandex.
This codebase was created to demonstrate a fully fledged backend application built with **[userver framework](https://userver.tech/)**

### Docker-compose
```
# Clone submodules
git submodule update --init

# Run service
make docker-start-service-release
```
### Local
```
make service-start-release
```
## Tests
Run unit and functional tests in docker or local.
### Docker-compose
```
make docker-test-release
```
### Local
```
make test-release
```

## Makefile

* `make build-debug` - debug build of the service with all the assertions and sanitizers enabled
* `make build-release` - release build of the service with LTO
* `make test-debug` - does a `make build-debug` and runs all the tests on the result
* `make test-release` - does a `make build-release` and runs all the tests on the result
* `make service-start-debug` - builds the service in debug mode and starts it
* `make service-start-release` - builds the service in release mode and starts it
* `make` or `make all` - builds and runs all the tests in release and debug modes
* `make format` - autoformat all the C++ and Python sources
* `make clean-` - cleans the object files
* `make dist-clean` - clean all, including the CMake cached configurations
* `make install` - does a `make build-release` and runs install in directory set in environment `PREFIX`
* `make install-debug` - does a `make build-debug` and runs install in directory set in environment `PREFIX`
* `make docker-COMMAND` - run `make COMMAND` in docker environment
* `make docker-build-debug` - debug build of the service with all the assertions and sanitizers enabled in docker environment
* `make docker-test-debug` - does a `make build-debug` and runs all the tests on the result in docker environment
* `make docker-build-release` - release build of the service with all the assertions and sanitizers enabled in docker environment
* `make docker-test-release` - does a `make build-release` and runs all the tests on the result in docker environment
* `make docker-start-service-release` - does a `make install-release` and runs service in docker environment
* `make docker-start-service-debug` - does a `make install-debug` and runs service in docker environment
* `make docker-clean-data` - stop docker containers and clean database data


## Authors project:

**Manager:**  
Viktoriia Berkholts <torina4@yandex.ru>


**Developers:**  
rumxcola <rumxcola@yandex.ru>  
Konstantin Artemev <rumxcola@yandex.ru>  
GasikPasik <sarbash2004@gmail.com>  
Anna Volkova <anna.mikhaylova09@yandex.ru>  
Nikita Semenov <nikitasemenov0388@gmail.com>  
Vadim Romanyuk <vadprof10@gmail.com>  
artemiev.kk <artemiev.kk@infotek.ru>  


**Maintainer:**  
Alexander Malkov <alexiprof@userver.tech>
