# RealMedium

[![urealmedium CI](https://github.com/userver-framework/realmedium_sample/actions/workflows/ci.yml/badge.svg?branch=develop)](https://github.com/userver-framework/realmedium_sample/actions/workflows/ci.yml)
[![urealmedium Docker build](https://github.com/userver-framework/realmedium_sample/actions/workflows/docker.yaml/badge.svg?branch=develop)](https://github.com/userver-framework/realmedium_sample/actions/workflows/docker.yaml)

This is project created in summer school Yandex.
This codebase was created to demonstrate a fully fledged backend application built with **[userver framework](https://userver.tech/)**

### Prerequisites

[Install the userver](https://userver.tech/de/dab/md_en_2userver_2build_2build.html#ways_to_get_userver) into the system
or
* check it out `git clone --depth 1 https://github.com/userver-framework/userver.git /path/to/userver`
* install all of its dependencies
* `ln -s /path/to/userver /path/to/realmedioum_sample/third_party/userver`

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
* `make start-debug` - builds the service in debug mode and starts it
* `make start-release` - builds the service in release mode and starts it
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
* `make docker-start-release` - does a `make install-release` and runs service in docker environment
* `make docker-start-debug` - does a `make install-debug` and runs service in docker environment
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
Elizaveta Rudneva <el1zarudneva@yandex.ru>


**Maintainer:**  
Alexander Malkov <alexiprof@userver.tech>
