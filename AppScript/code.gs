function doPost(e) {
  var sheet = SpreadsheetApp.getActiveSpreadsheet().getActiveSheet();
  var data = JSON.parse(e.postData.contents);
  sheet.appendRow([data.time, data.pres1, data.temp1, data.hum1, data.pres2, data.hum2, data.temp2, data.mass, data.amp]);
  return ContentService.createTextOutput("Success");
}

