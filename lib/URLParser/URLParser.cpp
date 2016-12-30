#include "URLParser.h"

#include "URL.h"

URL parseURL(String url)
{
    URL result;
    bool hasPort = false;
    int index = url.indexOf(':');
    if (index < 0)
    {
        return result;
    }

    result.protocol = url.substring(0, index);
    url.remove(0, (index + 3)); // remove http:// or https://

    index = url.indexOf('/');
    String host = url.substring(0, index);
    url.remove(0, index); // remove host part

    // get port
    index = host.indexOf(':');
    if (index >= 0)
    {
        result.host = host.substring(0, index); // hostname
        host.remove(0, (index + 1));            // remove hostname + :
        result.port = host.toInt();             // get port
        hasPort = true;
    }
    else
    {
        result.host = host;
    }
    if (!hasPort)
    {
        if (result.protocol.equals("http"))
        {
            result.port = 80;
        }
        else if (result.protocol.equals("https"))
        {
            result.port = 443;
        }
    }

    result.path = url;
    result.isValid = true;
}
