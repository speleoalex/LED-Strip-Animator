#!/bin/bash
cd "$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
appname="file:"$PWD"/index.html"
rm -rf chrome/*
mkdir chrome/Default
#http://peter.sh/experiments/chromium-command-line-switches/
#echo "file:/"$PWD"/index.html"
echo '
{
    "download":{"directory_upgrade": true,"extensions_to_open": "","prompt_for_download": true},
    "permissions":{"contentSettings":{"default_content_setting_values":{"geolocation":2}}}
}' > chrome/Default/Preferences
google-chrome  --disable-translate --no-first-run  --no-default-browser-check --app-window-size=1024,768 --user-data-dir=./chrome --disable-improved-download-protection --allow-running-insecure-content --app=$appname
rm -rf chrome/*

