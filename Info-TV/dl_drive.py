import io, os, shutil, time, http.client, socket
from googleapiclient.discovery import build
from oauth2client.service_account import ServiceAccountCredentials
from httplib2 import Http
from googleapiclient.errors import HttpError
from googleapiclient.http import MediaIoBaseDownload

#connection testing will loop as long as it gets answer from connected site
print("Checking connection ...", end="", flush=True)
while True:
	try:
		conn = http.client.HTTPSConnection("www.googleapis.com")
		conn.request("GET", "/")
		response = conn.getresponse()
		print(" OK!")
		break
	except socket.error:
		print(".", end="", flush=True)
	time.sleep(2)

scopes = ['https://www.googleapis.com/auth/drive.readonly']

#making credentials change the location and name of your credentials file
credentials = ServiceAccountCredentials.from_json_keyfile_name("./files/credentials.json", scopes)
http_auth = credentials.authorize(Http())

try:
    #using credentials to access shared folder + taking list of files on folder
    service = build('drive', 'v3', http=http_auth)
    request = service.files().list(fields="files(id, name, mimeType)").execute()
    files = request.get("files", [])
    
    #shortcut for used directories
    media=os.getcwd() + "/media"
    tmp=os.getcwd() + "/tmp"
    
    if files:
        #check if tmp folder exists, if not create one
        if not os.path.exists(tmp):
            os.makedirs(tmp)
        #list and download all files with difined mimeType    
        for file in files:
            if file['mimeType'] in ['image/png', 'image/jpg', 'image/jpeg']:
                print(u'{0} / id {1}'.format(file['name'], file['id']))
                request = service.files().get_media(fileId=file['id'])
                buffer = io.BytesIO()
                downloader = MediaIoBaseDownload(buffer, request)
                done = False
                while done is False:
                    status, done = downloader.next_chunk()
                    print("Download %d%%." % int(status.progress() * 100))
                #define folder and write the files
                handle = open(os.path.join(tmp, file['name']), "wb")
                handle.write(buffer.getvalue())
                handle.close()
    
        #check and delete folder with old media. rename tmp folder as media
        if os.path.exists(media):
            shutil.rmtree(media)
        os.rename(tmp, media)

#debug for errors        
except HttpError as error:
    # TODO(developer) - Handle errors from drive API.
    print(f'An error occurred: {error}')