
const {ccclass, property} = cc._decorator;

@ccclass
export default class ShaderTime extends cc.Component {
    _material: any;

    
    @property
    _max: number = 65535;

    isUpdate: boolean;

    @property
    get max(): number {
        return this._max;
    }
    set max(value) {
        this._max = value;
        if (!CC_EDITOR) {
            return;
        }
        
        let sprite = this.node.getComponent(cc.Sprite);
        if (sprite) {
            this._material = this.getComponent(cc.Sprite).sharedMaterials[0];
            if (this._material.effect._properties.time) {
                let material: any = sprite.sharedMaterials[0];
                material.effect.setProperty('time', value);
            }
        }
    }
    
    private _start = 0;

    protected update(dt) {
        this._material = this.node.getComponent(cc.Sprite).sharedMaterials[0];
        if (this.node.active && this._material && this._material.effect._properties.time) {
            this._setShaderTime(dt);
        }
    }
    private _setShaderTime(dt) {
        let start = this._start;
        if (start > this.max) start = 0;
        start += 0.02;
        this._material.effect.setProperty('time', start);

        this._start = start;
    }

    public setColor(color){
        // cc.log(this._material.effect._properties.time);
        if (this.node.active && this._material) {

            this._material.effect.setProperty('icolor0', cc.v4(color.r/255,color.g/255,color.b/255,color.a/255));

            return true;
        }

        return false;
    }
}
