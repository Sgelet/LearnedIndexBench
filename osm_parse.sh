#!/bin/bash

grep -o 'lon=".*"' | grep -o '[0-9\.]\+' | awk -F. '{print $1$2}'