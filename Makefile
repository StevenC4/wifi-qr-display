.PHONY: init

init:
	@echo "#pragma once" > ./src/secrets.h
	@echo "" >> ./src/secrets.h
	@echo "const char* WIFI_SSID = \"your-network-ssid\";" >> ./src/secrets.h
	@echo "const char* WIFI_PASSWORD = \"your-network-password\";" >> ./src/secrets.h
