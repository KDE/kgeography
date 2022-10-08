function getProperty (prop, key) {
    return _dict_[key][prop];
}
Ts.setcall("get-case", getProperty);
 
_dict_ = {};
function addDictCases (key, pos) {
    if (!_dict_[key])
        _dict_[key] = {};
    _dict_[key]["possesion"]     = pos;   
}
 
// dictionary entries follow:
addDictCases("സംസ്ഥാനങ്ങള്‍", "സംസ്ഥാനങ്ങളുടെ");
addDictCases("രാജ്യങ്ങള്‍", "രാജ്യങ്ങളുടെ");
addDictCases("സംസ്ഥാനങ്ങളും വിഭാഗങ്ങളും", "സംസ്ഥാനങ്ങളുടേയും വിഭാഗങ്ങളുടേയും");
addDictCases("സംസ്ഥാനങ്ങളും ഭരണപ്രദേശങ്ങളും", "സംസ്ഥാനങ്ങളുടേയും ഭരണപ്രദേശങ്ങളുടേയും");
addDictCases("പ്രവിശ്യകള്‍", "പ്രവിശ്യകളുടെ");
addDictCases("പ്രദേശങ്ങള്‍", "പ്രദേശങ്ങളുടെ");
addDictCases("ജില്ലകള്‍", "ജില്ലകളുടെ");
addDictCases("വിഭാഗങ്ങള്‍", "വിഭാഗങ്ങളുടെ");
addDictCases("കൌണ്ടികള്‍", "കൌണ്ടികളുടെ");
addDictCases("മുനിസിപ്പാലിറ്റികള്‍", "മുനിസിപ്പാലിറ്റികളുടെ");
addDictCases("പെരിഫറികള്‍", "പെരിഫറികളുടെ");
addDictCases("പാരിഷസ്", "പാരിഷസുകളുടെ");
addDictCases("ഗവര്‍ണറേറ്റുകള്‍", "ഗവര്‍ണറേറ്റുകളുടെ");
addDictCases("മഹാസമുദ്രങ്ങളും താഴ്വരകളും", "മഹാസമുദ്രങ്ങളുടേയും താഴ്വരകളുടേയും");
addDictCases("പ്രിഫക്ച്ചറുകള്‍", "പ്രിഫക്ച്ചറുകളുടെ");
addDictCases("ഒബ്ലാസ്റ്റുകള്‍", "ഒബ്ലാസ്റ്റുകളുടെ");
addDictCases("കോമാര്‍കെസ്", "കോമാര്‍കെസുകളുടെ");
addDictCases("ഭൂഖണ്ടങ്ങള്‍", "ഭൂഖണ്ടങ്ങളുടെ");
addDictCases("കാന്റോണ്‍സ്", "കാന്റോണ്‍സുകളുടെ");
addDictCases("എമിറൈറ്റ്സ്", "എമിറൈറ്റ്സുകളുടെ");