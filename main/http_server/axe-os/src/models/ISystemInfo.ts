import { eASICModel } from './enum/eASICModel';
import { IHistory } from 'src/models/IHistory';

export interface ISystemInfo {

    flipscreen: number;
    invertscreen: number;
    autoscreenoff: number;
    power: number,
    maxPower: number,
    minPower: number,
    voltage: number,
    maxVoltage: number,
    minVoltage: number,
    current: number,
    temp: number,
    vrTemp: number,
    hashRateTimestamp: number,
    hashRate: number,
    hashRate_10m: number,
    hashRate_1h: number,
    hashRate_1d: number,
    bestDiff: string,
    bestSessionDiff: string,
    freeHeap: number,
    coreVoltage: number,
    hostname: string,
    ssid: string,
    wifiPass: string,
    wifiStatus: string,
    sharesAccepted: number,
    sharesRejected: number,
    uptimeSeconds: number,
    asicCount: number,
    smallCoreCount: number,
    ASICModel: eASICModel,
    deviceModel: string,
    stratumURL: string,
    stratumPort: number,
    stratumUser: string,
    frequency: number,
    version: string,
    invertfanpolarity: number,
    autofanspeed: number,
    fanspeed: number,
    fanrpm: number,
    coreVoltageActual: number,
    lastResetReason: string,
    jobInterval: number,

    boardtemp1?: number,
    boardtemp2?: number,
    overheat_temp: number,

    nonceDistribution: number[],

    history: IHistory
}
