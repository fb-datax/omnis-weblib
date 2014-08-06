/**
 * @class dorgcomps.abstract
 *
 * Basis Klasse für alle Komponenten
 */
var weblibcomps = weblibcomps || {};
weblibcomps.abstract = function() {};
weblibcomps.abstract.prototype = {

	// Vesionsinfo fuer die lib
	ident: "abstract",
	version: "1.0.0",
	apiVersion: "1",
	buildDate: "20130419-1201",
	buildNumber: "9",
	initialized: false,

	/**
	 * Init Function für die lbs
	 */
	init: function() {

		if (this.initialized === true) {
			console.warn(this.ident + " ist bereits initialisiert...");
		}

		//vermerken
		this.initialized.initialized = true;

		// dorgv4 informieren
		if (weblib && weblib.doCompInit) {
			weblib.doCompInit(this.ident);
		}
	},

	/**
	 * Wurde die lib bereits initailisiert
	 *
	 * @return {Boolean} initialisiert ja/nein
	 */
	isInitialized: function() {
		return this.initialized;
	},
	/**
	 * Setzt für Daten für die übergebene Komponente
	 *
	 * @param {String} pData  [description]
	 */
	setData: function(pData) {
		return;
	},

	/**
	 * Gibt die Daten an die Anwendung zurück
	 * @return {String}
	 */
	getData: function() {
		return "";
	},
	/**
	 * Custom Impl in der jeweiligen Komponente
	 *
	 * @param {String} pType
	 * @param {String} pParam1
	 * @param {String} pParam2
	 * @param {String} pParam3
	 * @param {String} pParam4
	 * @param {String} pParam5
	 * @param {String} pParam6
	 * @param {String} pParam7
	 * @param {String} pParam8
	 * @param {String} pParam9
	 *
	 */
	doCustomAction: function(pType, pParam1, pParam2, pParam3, pParam4, pParam5, pParam6, pParam7, pParam8, pParam9) {
		return;
	},
	/**
	 * Custom Impl in der jeweiligen Komponente
	 *
	 * @param {String} pType
	 * @param {String} pParam1
	 * @param {String} pParam2
	 * @param {String} pParam3
	 * @param {String} pParam4
	 * @param {String} pParam5
	 * @param {String} pParam6
	 * @param {String} pParam7
	 * @param {String} pParam8
	 * @param {String} pParam9
	 *
	 */
	sendCustomAction: function(pType, pParam1, pParam2, pParam3, pParam4, pParam5, pParam6, pParam7, pParam8, pParam9) {
		if (weblib && weblib.doCompCustAction) {
			weblib.doCompCustAction(this.ident, pType, pParam1, pParam2, pParam3, pParam4, pParam5, pParam6, pParam7, pParam8, pParam9);
		}
	}
};