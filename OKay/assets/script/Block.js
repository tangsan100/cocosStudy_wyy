
cc.Class({
    extends: cc.Component,

    properties: {
       
    },



    onLoad () {
        this.hadSetColor = false;
        this.shaderObj = this.node.getComponent("ShaderTime");
    },


    start () {

    },

    update (dt) {
        if (!this.hadSetColor)
            this.hadSetColor = this.shaderObj.setColor(this.node.color)
    },
});
