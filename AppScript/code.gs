function doPost(e) {
  var sheet = SpreadsheetApp.getActiveSpreadsheet().getActiveSheet();
  var data = JSON.parse(e.postData.contents);
  sheet.appendRow([data.time, data.temp, data.hum, data.pres, data.mass]);
  return ContentService.createTextOutput("Success");
}
