#!/bin/sh

nasm -o sas.mgf -f bin sas.asm

cp sas.mgf ../ProjectStoneship/sas.mgf
cp sas.mgf ../ProjectStoneship/Debug/sas.mgf


nasm -o if.mgf -f bin if.asm

cp if.mgf ../ProjectStoneship/if.mgf
cp if.mgf ../ProjectStoneship/Debug/if.mgf