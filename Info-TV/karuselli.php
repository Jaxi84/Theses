<!--start with html-->
<!DOCTYPE html>
<html lang="en">
<head>
<!--create head, page refresh and load necessary libraries-->
  <title>InfoTV title here</title>
  <meta http-equiv="refresh" content="20" />
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <link rel="stylesheet" href="./lib/bootstrap.min.css"/>
  <script src="./lib/jquery.min.js"></script>
  <script src="./lib/bootstrap.min.js"></script>
</head>

<body>
<!--creating body, adding week number-->    
<h1 class="text-center">I'am InfoTV - Week: <?=date("W");?></h1>
<!--creating bootstrap carousel with chosen specs-->
<div class="container">
    <div id="tttv" class="carousel slide" data-bs-ride="carousel" data-bs-interval="1000">
        <div class="carousel-inner">             
<?php
//creating filelist for carousel from downloaded media folder
//the names and amount of contet will be dynamic 
foreach (glob("media/{*.png,*.jpg,*.jpeg}", GLOB_BRACE) as $i => $image) {
?>
            <div class="carousel-item<?=($i == 0) ? " active" : null;?>">
                <img class="d-block w-100" src="<?=$image;?>?<?=filemtime($image);?>"/>
            </div>
<?php
}
?>
        </div>
    </div>
</div>
</body>
</html>