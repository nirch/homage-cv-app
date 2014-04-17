//
//  CameraControllerMac.h
//  ImageAcquisition
//
//  Created by Oren Shtark on 4/28/13.
//
//

#import <QTKit/QTKit.h>
#import "CameraController.h"

@interface CameraControllerMac : CameraController {
    QTCaptureSession *captureSession;
}

@property (nonatomic, retain) QTCaptureSession *captureSession;

@end
