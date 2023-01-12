<?php
//creating menu
require_once "./files/simplehtmldom.php";
$html = file_get_html("[insert URL for restaurant's menu here]");

//select chosen titles and edit them as plaintext you probably need to adjust this. Look from page's source
$e = null;
$e .= $html->find("#panel-41-1-0-0 .widget-title",0)->plaintext . PHP_EOL . PHP_EOL;
//$e .= "\n\n ";
// . PHP_EOL . PHP_EOL;
foreach ($html->find("#panel-41-1-0-0 .textwidget h4") as $weekday) {

    //weekday
    $e .= $weekday->plaintext . PHP_EOL . PHP_EOL;
    //$e .= "\n\n ";
    //next on the list after weekday are food names
    $e .= $weekday->next_sibling()->plaintext . PHP_EOL . PHP_EOL;
    //$e .= "\n\n ";
        
}
//*** making image ***
//creating a backround for and choosing colors
$im = imagecreatetruecolor(1000, 1100);
$lgrey = imagecolorallocate($im, 192, 192, 192);
$shadow = imagecolorallocate($im, 128, 128, 128);
$black = imagecolorallocate($im, 0, 0, 0);
imagefilledrectangle($im, 0, 0, 999, 1099, $lgrey);

//print text from menu and choose font
$text = $e;
$font = "./files/arial.ttf";

//font and shadow colors
imagettftext($im, 12, 0, 301, 31, $shadow, $font, $text);
imagettftext($im, 12, 0, 300, 30, $black, $font, $text);

//create image, clean it from memory
imagepng($im, "./files/restaurant_menu.png");
imagedestroy($im);

//*** uploading menu.png to gdrive ***
//libraries, api and authentication + scope //Check the folder and crentials file name
require_once "./vendor/autoload.php";
putenv("GOOGLE_APPLICATION_CREDENTIALS=./files/credentials.json");
use Google\Service\Drive;
$client = new Google_Client();
$client->useApplicationDefaultCredentials();
$client->addScope([Google\Service\Drive::DRIVE]);
$service = new Drive($client);

//create file on a folder
//check the file id and insert it below
$exists = $service->files->listFiles([
    "includeItemsFromAllDrives" => true,
    "supportsAllDrives" => true,
    "q" => "name = 'restaurant_menu.png' and '[insert file id here]' in parents"
]);

//if file already exists write picture on it
if (!empty($exists[0]['id'])) {

    $service->files->update($exists[0]['id'], new Drive\DriveFile(), [
        "data" => file_get_contents("./files/restaurant_menu.png"),
        "mimeType" => mime_content_type("./files/restaurant_menu.png"),
        "uploadType" => "multipart"
    ]);

//if file does not exists create new file on shared folder and write data in it
} else {

    $service->files->create(new Drive\DriveFile([
        "name" => "restaurant_menu.png",
        "parents" => ["[insert file id here]"]
    ]), [
        "data" => file_get_contents("./files/restaurant_menu.png"),
        "mimeType" => mime_content_type("./files/restaurant_menu.png"),
        "uploadType" => "multipart"
    ]);
}