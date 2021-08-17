
cc.Class({
    extends: cc.Component,

    properties: {
        manifestUrl: {
            type: cc.Asset,
            default: null
        },
        fileLabel:{
            type: cc.Label,
            default: null,
        },
        byteLabel:{
            type: cc.Label,
            default: null,
        },
        infoLabel:{
            type: cc.Label,
            default: null,
        },
        urlLabel:{
            type: cc.Label,
            default: null,
        },
        btnUpdate:{
            type: cc.Node,
            default: null,
        },
        btnCheck:{
            type: cc.Node,
            default: null,
        },
        node_parent:{
            type: cc.Node,
            default: null,
        },
        _updating: false,
        _canRetry: false,
        _storagePath: ''
    },

     // use this for initialization
     onLoad: function () {

        // Hot update is only available in Native build
        if (!cc.sys.isNative) {
            this.node.active = false
            return;
        }
        this.autoUpdate = true //是否自动更新
        this.isUpdateCompleted = false //是否更新完成
        this._storagePath = ((jsb.fileUtils ? jsb.fileUtils.getWritablePath() : '/') + 'blackjack-remote-asset');
        cc.log('Storage path for remote asset : ' + this._storagePath);

        // Setup your own version compare handler, versionA and B is versions in string
        // if the return value greater than 0, versionA is greater than B,
        // if the return value equals 0, versionA equals to B,
        // if the return value smaller than 0, versionA is smaller than B.
        this.versionCompareHandle = function (versionA, versionB) {
            cc.log("JS Custom Version Compare: version A is " + versionA + ', version B is ' + versionB);
            var vA = versionA.split('.');
            var vB = versionB.split('.');
            for (var i = 0; i < vA.length; ++i) {
                var a = parseInt(vA[i]);
                var b = parseInt(vB[i] || 0);
                if (a === b) {
                    continue;
                }
                else {
                    return a - b;
                }
            }
            if (vB.length > vA.length) {
                return -1;
            }
            else {
                return 0;
            }
        };

        // Init with empty manifest url for testing custom manifest
        this._am = new jsb.AssetsManager('', this._storagePath, this.versionCompareHandle);

        // var panel = this.panel;
        // Setup the verification callback, but we don't have md5 check function yet, so only print some message
        // Return true if the verification passed, otherwise return false
        this._am.setVerifyCallback(function (path, asset) {
            // When asset is compressed, we don't need to check its md5, because zip file have been deleted.
            var compressed = asset.compressed;
            // Retrieve the correct md5 value.
            var expectedMD5 = asset.md5;
            // asset.path is relative path and path is absolute.
            var relativePath = asset.path;
            // The size of asset file, but this value could be absent.
            var size = asset.size;
            if (compressed) {
                // panel.info.string = "Verification passed : " + relativePath;
                console.log("Verification passed :" + relativePath);
                return true;
            }
            else {
                // panel.info.string = "Verification passed : " + relativePath + ' (' + expectedMD5 + ')';
                console.log("Verification passed :" + relativePath + ' (' + expectedMD5 + ')');
                return true;
            }
        });

        // this.panel.info.string = 'Hot update is ready, please check or directly update.';

        if (cc.sys.os === cc.sys.OS_ANDROID) {
            // Some Android device may slow down the download process when concurrent tasks is too much.
            // The value may not be accurate, please do more test and find what's most suitable for your game.
            this._am.setMaxConcurrentTask(2);
            // this.panel.info.string = "Max concurrent tasks count have been limited to 2";
            console.log("Max concurrent tasks count have been limited to 2");
        }
        
        // this.panel.fileProgress.progress = 0;
        // this.panel.byteProgress.progress = 0;

        if (this.autoUpdate) {
            this.node_parent.active = false
            this.checkUpdate();
        }
       
    },

    checkCb: function (event) {
        cc.log('Code: ' + event.getEventCode());
        switch (event.getEventCode())
        {
            case jsb.EventAssetsManager.ERROR_NO_LOCAL_MANIFEST:
                this.infoLabel.string = "找不到本地清单文件，已跳过热更新.";
                console.log("No local manifest file found, hot update skipped.");
                break;
            case jsb.EventAssetsManager.ERROR_DOWNLOAD_MANIFEST:
            case jsb.EventAssetsManager.ERROR_PARSE_MANIFEST:
                this.infoLabel.string = "下载清单文件失败，已跳过热更新.";
                console.log("Fail to download manifest file, hot update skipped.");
                break;
            case jsb.EventAssetsManager.ALREADY_UP_TO_DATE:
                this.infoLabel.string = "已经是最新的版本了！" + Math.floor(Math.random()*100);
                this.isUpdateCompleted = true
                if (this.autoUpdate) {
                    this.node.active = false
                    this.node_parent.active = false
                }
                console.log("Already up to date with the latest remote version.");
                //cc.find("Canvas").getComponent("SceneLogin").toAutoLogin();
                break;
            case jsb.EventAssetsManager.NEW_VERSION_FOUND:
                console.log("New version found, please try to update.");
                // this.infoLabel.string = 'New version found, please try to update.';
                this.infoLabel.string = '检测到新版本，\n请更新。';
                this.isUpdateCompleted = false
                this.node_parent.active = true
                // this.panel.checkBtn.active = false;
                // this.panel.fileProgress.progress = 0;
                // this.panel.byteProgress.progress = 0;
                if (this.autoUpdate) {
                    this.btnCheck.active = false;
                    this.btnUpdate.active = true;
                    this.btnUpdate.x = 0
                }else{
                    this.btnCheck.active = true;
                    this.btnUpdate.active = true;
                }
               
                break;
            default:
                return;
        }
        
        this._am.setEventCallback(null);
        this._checkListener = null;
        this._updating = false;
    },

    updateCb: function (event) {
        var needRestart = false;
        var failed = false;
        switch (event.getEventCode())
        {
            case jsb.EventAssetsManager.ERROR_NO_LOCAL_MANIFEST:
                this.infoLabel.string = '找不到本地清单文件，已跳过热更新.';
                console.log('No local manifest file found, hot update skipped.');
                failed = true;
                break;
            case jsb.EventAssetsManager.UPDATE_PROGRESSION:
                console.log("更新总百分比"+ event.getPercent());
                console.log("更新文件总百分比" + event.getPercentByFile());
                console.log("下载文件：" + event.getDownloadedFiles() + "/总文件:" + event.getTotalFiles());
                console.log("下载字节：" + event.getDownloadedBytes() + "/总字节:" + event.getTotalBytes());
                // this.panel.byteProgress.progress = event.getPercent();
                // this.panel.fileProgress.progress = event.getPercentByFile();

                this.fileLabel.string = event.getDownloadedFiles() + ' / ' + event.getTotalFiles();
                this.byteLabel.string = event.getDownloadedBytes() + ' / ' + event.getTotalBytes();

                // var msg = event.getMessage();
                // if (msg) {
                //     this.infoLabel.string = 'Updated file: ' + msg;
                // }
                if (this.autoUpdate) {
                    this.btnUpdate.active = false
                }
                this.infoLabel.string = "正在更新中...";
                break;
            case jsb.EventAssetsManager.ERROR_DOWNLOAD_MANIFEST:
            case jsb.EventAssetsManager.ERROR_PARSE_MANIFEST:
                this.infoLabel.string = '下载清单文件失败，已跳过热更新.';
                console.log("Fail to download manifest file, hot update skipped.");
                failed = true;
                break;
            case jsb.EventAssetsManager.ALREADY_UP_TO_DATE:
                this.infoLabel.string = "已经是最新的版本了！" + Math.floor(Math.random()*100);
                this.isUpdateCompleted = true
                console.log("Already up to date with the latest remote version.");
                failed = true;
                break;
            case jsb.EventAssetsManager.UPDATE_FINISHED:
                this.infoLabel.string = '完成更新. ' + event.getMessage();
                this.fileLabel.string = event.getTotalFiles() + ' / ' + event.getTotalFiles();
                this.byteLabel.string = event.getTotalBytes() + ' / ' + event.getTotalBytes();
                this.isUpdateCompleted = true
                console.log("Update finished. " + event.getMessage());
                needRestart = true;
                break;
            case jsb.EventAssetsManager.UPDATE_FAILED:
                this.infoLabel.string = '更新失败. ' + event.getMessage();
                // this.panel.retryBtn.active = true;
                console.log("Update failed. " + event.getMessage());
                this._updating = false;
                this._canRetry = true;
                break;
            case jsb.EventAssetsManager.ERROR_UPDATING:
                this.infoLabel.string = 'Asset update error: ' + event.getAssetId() + ', ' + event.getMessage();
                console.log("Asset update error: " + event.getMessage());
                break;
            case jsb.EventAssetsManager.ERROR_DECOMPRESS:
                this.infoLabel.string = event.getMessage();
                console.log(event.getMessage());
                break;
            default:
                break;
        }

        if (failed) {
            this._am.setEventCallback(null);
            this._updateListener = null;
            this._updating = false;
        }

        if (needRestart) {
            this._am.setEventCallback(null);
            this._updateListener = null;
            // Prepend the manifest's search path
            var searchPaths = jsb.fileUtils.getSearchPaths();
            var newPaths = this._am.getLocalManifest().getSearchPaths();
            console.log(JSON.stringify(newPaths));
            Array.prototype.unshift.apply(searchPaths, newPaths);
            // This value will be retrieved and appended to the default search path during game startup,
            // please refer to samples/js-tests/main.js for detailed usage.
            // !!! Re-add the search paths in main.js is very important, otherwise, new scripts won't take effect.
            cc.sys.localStorage.setItem('HotUpdateSearchPaths', JSON.stringify(searchPaths));
            jsb.fileUtils.setSearchPaths(searchPaths);

            if (this.autoUpdate) {
                cc.audioEngine.stopAll();
                cc.game.restart();
            }
        }
    },

    

    // retry: function () {
    //     if (!this._updating && this._canRetry) {
    //         this.panel.retryBtn.active = false;
    //         this._canRetry = false;
            
    //         this.panel.info.string = 'Retry failed Assets...';
    //         this._am.downloadFailedAssets();
    //     }
    // },
    
    //检查更新按钮
    checkUpdate: function () {
        if (this._updating) {
            this.infoLabel.string = '检查更新按钮。。。。';
            console.log("正在更新中")
            return;
        }
        if (this._am.getState() === jsb.AssetsManager.State.UNINITED) {
            // Resolve md5 url
            // console.log("this.manifestUrl.nativeUrl="+this.manifestUrl.nativeUrl);
            var url = this.manifestUrl.nativeUrl;
            // console.log("url="+url);
            if (cc.loader.md5Pipe) {
                url = cc.loader.md5Pipe.transformURL(url);
            }
            // console.log("url="+url);
            this.urlLabel.string = this.manifestUrl.nativeUrl+"\n###\n"+url;

            this._am.loadLocalManifest(url);
        }
        if (!this._am.getLocalManifest() || !this._am.getLocalManifest().isLoaded()) {
            this.infoLabel.string = 'Failed to load local manifest ...';
            return;
        }
        this._am.setEventCallback(this.checkCb.bind(this));

        this._am.checkUpdate();
        this._updating = true;
    },

    //更新按钮
    hotUpdate: function () {
        if (this.isUpdateCompleted) {
            this.infoLabel.string = '更新已完成 ...';
            this.node.active = false;
            cc.game.end();
            return
        }

        if (this._am && !this._updating) {
            this._am.setEventCallback(this.updateCb.bind(this));

            if (this._am.getState() === jsb.AssetsManager.State.UNINITED) {
                // Resolve md5 url
                var url = this.manifestUrl.nativeUrl;
                if (cc.loader.md5Pipe) {
                    url = cc.loader.md5Pipe.transformURL(url);
                }
                this._am.loadLocalManifest(url);
            }

            this._failCount = 0;
            this._am.update();
            // this.panel.updateBtn.active = false;
            this._updating = true;
        }
    },

    onDestroy: function () {
        if (this._updateListener) {
            this._am.setEventCallback(null);
            this._updateListener = null;
        }
    }
});
