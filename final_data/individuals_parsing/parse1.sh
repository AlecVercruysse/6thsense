#!/bin/bash
sed -r 's/-{2,}/, /g' > p.txt
sed -r -i 's/(, )+/, /g' p.txt
