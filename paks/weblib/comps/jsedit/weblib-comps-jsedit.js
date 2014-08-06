/**
 * Component
 * @ignore
 **/

var weblibcomps = weblibcomps || {};
weblibcomps.jsedit = new weblibcomps.abstract();
weblibcomps.jsedit.ident = "jsedit";

/**
 * Custom Impl
 */
weblibcomps.jsedit.setData = function(pData) {
	editor.setValue(pData);
	editor.clearSelection();
	editor.gotoLine(1);
};

/**
 * Custom Impl
 */
weblibcomps.jsedit.getData = function() {
	return editor.getValue();
};


weblibcomps.jsedit.doCustomAction = function(pType, pParam1, pParam2, pParam3, pParam4, pParam5, pParam6, pParam7, pParam8, pParam9) {
	console.log(pType, pParam1);
	switch (pType) {
		case '#THEME':
			console.log("ace/theme/" + pParam1);
			editor.setTheme("ace/theme/" + pParam1);
			break;

		case '#READONLY':
			if (pParam2 === 'true') {
				editor.setReadOnly(true);
			} else {
				editor.setReadOnly(false);
			}

			break;
		case '#MODE':
			editor.getSession().setMode("ace/mode/" + pParam1);
			break;
	}
};