_genitif_ = {}
_genitif_['de'] = 'de '
_genitif_['du'] = 'du '
_genitif_['d'] = 'd\''
_genitif_['de l'] = 'de l\''
_genitif_['de l\''] = 'de l\''
_genitif_['de la'] = 'de la '
_genitif_['des'] = 'des '

_datif_ = {}
_datif_['de'] = 'à '
_datif_['du'] = 'au '
_datif_['d'] = 'à '
_datif_['de l'] = 'à l\''
_datif_['de la'] = 'à la '
_datif_['des'] = 'aux '

_nominatif_ = {}
_nominatif_['de'] = ''
_nominatif_['du'] = 'le '
_nominatif_['d'] = ''
_nominatif_['de l'] = 'l\''
_nominatif_['de la'] = 'la '
_nominatif_['des'] = 'les '

_directif_ = {}
_directif_['de'] = 'à '
_directif_['du'] = 'au '
_directif_['d'] = 'en '
_directif_['de l'] = 'en '
_directif_['de la'] = 'en '
_directif_['des'] = 'aux '

_forms_ = {}
_forms_['directif'] = _directif_
_forms_['nominatif'] = _nominatif_
_forms_['datif'] = _datif_
_forms_['genitif'] = _genitif_

_genitif_ = {};
function addDictGenderLike (key, base) {
	_genitif_[key] = base;
}

function dynamicSetGenderLike (base) {
	addDictGenderLike(Ts.msgstrf(), base);
	Ts.fallback();
}
Ts.setcall("pose-genre-etendu", dynamicSetGenderLike);

function getGenderLike (cas, key) {
	gender_like = _genitif_[key];
	if ( gender_like )
	{
		caster = _forms_[cas];
		if ( caster )
		{
			ret = caster[gender_like] + key;
		}
		else
		{
			ret = gender_like + key;
		}
	}
	else
	{
		ret = key;
	}
	return ret
}

Ts.setcall("get-genre-etendu", getGenderLike);
Ts.setcall("majus-initiale", Ts.toUpperFirst);
