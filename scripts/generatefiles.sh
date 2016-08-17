#!/bin/sh
echo "const QString g_tlsPoolGuiVersion = \"`git describe --long --dirty`\";" > $1
