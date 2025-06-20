.PHONY: init

init:
	@echo "#include secrets.h" > ./src/secrets.cpp
	@echo "" >> ./src/secrets.cpp
	@echo "const char* WIFI_SSID = \"your-network-ssid\";" >> ./src/secrets.cpp
	@echo "const char* WIFI_PASSWORD = \"your-network-password\";" >> ./src/secrets.cpp
	@echo "const char* SERVER_URL = \"http://your-server-url/route\";" >> ./src/secrets.cpp
	@echo "const char* SHARED_SECRET = \"supersecretkey\";" >> ./src/secrets.cpp
