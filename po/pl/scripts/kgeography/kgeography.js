// Polish language functions for Kgeography

function getCase (prop, key) {
	return Ts.getProp(key, prop);
}

function getCaseLower (prop, key) {
	str = Ts.getProp(key, prop);
	return str.toLowerCase();
}

// Directory entry with first capital letter
Ts.setcall("przypadek-wielkie", getCase);

// Lower case directory entry
Ts.setcall("przypadek-male", getCaseLower);

// Load cases for different region types
Ts.loadProps("general");