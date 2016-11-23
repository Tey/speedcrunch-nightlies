# Tested on Debian-based systems (Ubuntu 16.04). Your package names may vary slightly.
sudo apt-get install transifex-client gettext intltool-debian
pip install urllib3 --upgrade
TRANSIFEXRCFILE="$HOME/.transifexrc"
if [ ! -f $TRANSIFEXRCFILE ]; then
    echo -e \
"[https://www.transifex.com]\n"\
"hostname = https://www.transifex.com\n"\
"username = <USERNAME>\n"\
"password = <PASSWORD>\n"\
"token =\n" > $TRANSIFEXRCFILE
    echo File $TRANSIFEXRCFILE created. Please edit it accordingly before proceeding.
fi

