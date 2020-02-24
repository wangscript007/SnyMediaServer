# MediaServer

### getDirectoryContent

- This  request is used to list directory content in remote machine running   MediaServer.
- Request uses `HTTP Post` Method to post data in JSON format

Example

```shell
curl -d "@data.json" -X POST http://localhost:8000/api/getDirectoryContent
```

`data.json` may like this:

```json
{
	"path": "/Users/streamingnology/Documents/Media",
    "showHiddenContent":"false"
}
```

Respons from MediaServer may like this below:

```json
{
	"status": 200,
	"data": [{
		"label": "现代世界的天才马克思.mp4",
		"path": "/Users/streamingnology/Documents/docroot/media/现代世界的天才马克思.mp4",
		"last": true
	}, {
		"label": "[哥伦比亚广播公司：2012年度维多利亚的秘密内衣秀].CBS.The.Victoria's.Secret.Fashion.Show.2012-HDTV-2HD.mp4",
		"path": "/Users/streamingnology/Documents/docroot/media/[哥伦比亚广播公司：2012年度维多利亚的秘密内衣秀].CBS.The.Victoria's.Secret.Fashion.Show.2012-HDTV-2HD.mp4",
		"last": true
	}, {
		"label": "2016年维多利亚的秘密时尚内衣秀.HDTV720P.X264.AAC.中英字幕.mp4",
		"path": "/Users/streamingnology/Documents/docroot/media/2016年维多利亚的秘密时尚内衣秀.HDTV720P.X264.AAC.中英字幕.mp4",
		"last": true
	}, {
		"label": "data.json",
		"path": "/Users/streamingnology/Documents/docroot/media/data.json",
		"last": true
	}, {
		"label": "[法拉利.F2012.主设计师Nikolas.Tombazis作技术讲解].Singleseater.Technical.Explanations.mp4",
		"path": "/Users/streamingnology/Documents/docroot/media/[法拉利.F2012.主设计师Nikolas.Tombazis作技术讲解].Singleseater.Technical.Explanations.mp4",
		"last": true
	}, {
		"label": "维多利亚的秘密：泳装特辑2015.特效中字.The.Victorias.Secret.Swim.Special.2015.BD720P.X264.AAC.English.CHS.Mp4Ba.mp4",
		"path": "/Users/streamingnology/Documents/docroot/media/维多利亚的秘密：泳装特辑2015.特效中字.The.Victorias.Secret.Swim.Special.2015.BD720P.X264.AAC.English.CHS.Mp4Ba.mp4",
		"last": true
	}],
	"msg": "操作成功"
}
```

### addToVODManager 

- This request is used to add media file you want to playback to VOD Manager. **You must add media file to VOD Manager before you playback.**
- Request uses `HTTP Post` Method to post data in JSON format

 Example

```shell
curl -d "@data.json" -X POST http://localhost:8000/api/addToVODManager
```

`data.json` may like this:

```json
{
	"path":"/Users/streamingnology/Documents/docroot/media/[哥伦比亚广播公司：2012年度维多利亚的秘密内衣秀].CBS.The.Victoria's.Secret.Fashion.Show.2012-HDTV-2HD.mp4"
}
```

Respons from MediaServer may like this below:

```json
{
	"playlist": [{
			"protocol": "hls",
			"uri ": "http://localhost:8000/vod/8c74123b9cdbda5eb91f340995931e06/playlist.m3u8"
		},
		{
			"protocol": "dash",
			"uri ": "http://localhost:8000/vod/8c74123b9cdbda5eb91f340995931e06/playlist.mpd"
		}
	]
}
```

