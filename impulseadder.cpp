#include "impulseadder.h"

ImpulseAdder::ImpulseAdder(QGraphicsScene *scene):
    scene(scene)
{
    body = Body::getBodyInstance();
    mioSuit = MioSuit::getMioSuitInstance();
    wasBodyPartClicked = true;
}

ImpulseAdder::~ImpulseAdder()
{
    body->removePopFromScene(scene, partID);
    mioSuit->removePopFromScene(scene);
}

void ImpulseAdder::setPopBodyPart(int partID)
{
    body->addPopToScene(scene, partID, this);
    this->partID = partID;
    mioSuit->addPopToScene(partID, scene);
    if (body->getPopStatus(partID) == AbstractBodyPart::BothDisabled) {
        mioSuit->resetPopDotsPairs();
    }
}

int ImpulseAdder::getPartID() const
{
    return partID;
}

void ImpulseAdder::onBodyPartClicked(bool isFirstMuscle)
{
    wasBodyPartClicked = true;
    if (body->getPopStatus(partID) == AbstractBodyPart::BothEnabled) {
        if (mioSuit->isMuscleActive(true) && isFirstMuscle) {
            mioSuit->activateMusclePopDotsPairs(false);
        } else if (mioSuit->isMuscleActive(false) && !isFirstMuscle) {
            mioSuit->activateMusclePopDotsPairs(true);
        } else {
            if (isFirstMuscle) {
                body->setPopStatus(partID, AbstractBodyPart::SecondEnabled);
            } else {
                body->setPopStatus(partID, AbstractBodyPart::FirstEnabled);
            }
        }
    }
    if (body->getPopStatus(partID) == AbstractBodyPart::FirstEnabled) {
        mioSuit->activateMusclePopDotsPairs(true);
        mioSuit->deactivateMusclePopDotsPairs(false);
    }
    if (body->getPopStatus(partID) == AbstractBodyPart::SecondEnabled) {
        mioSuit->activateMusclePopDotsPairs(false);
        mioSuit->deactivateMusclePopDotsPairs(true);
    }
    if (body->getPopStatus(partID) == AbstractBodyPart::BothDisabled) {
        mioSuit->resetPopDotsPairs();
    }
}

void ImpulseAdder::onBackgroundClicked()
{
    if (wasBodyPartClicked){
        wasBodyPartClicked = false;
        return;
    }
    wasBodyPartClicked = true;
    if (!mioSuit->isPopValuesChanged()) {
        emit done(2);
    } else {
        QMessageBox msgBox;
        msgBox.setWindowIcon(QIcon(":/icons/icons/reforma1.ico"));
        msgBox.setText(tr("Save changes?"));
        msgBox.setIcon(QMessageBox::Question);
        QPushButton *yesBtn = msgBox.addButton(tr("Yes"), QMessageBox::ActionRole);
        QPushButton *noBtn = msgBox.addButton(tr("No"), QMessageBox::ActionRole);
        msgBox.setDefaultButton(msgBox.addButton(tr("Continue"), QMessageBox::ActionRole));
        msgBox.exec();

        if (msgBox.clickedButton() == (QAbstractButton *) yesBtn) {
            lastCheck();
            emit done(1);
        } else if (msgBox.clickedButton() == (QAbstractButton *) noBtn) {
            emit done(0);
        }
    }
}

void ImpulseAdder::lastCheck(){
    if (mioSuit->isMuscleActive(true) && mioSuit->isMuscleActive(false)){
        body->setPopStatus(partID, AbstractBodyPart::BothEnabled);
        return;
    }
    if (mioSuit->isMuscleActive(true)){
        body->setPopStatus(partID, AbstractBodyPart::FirstEnabled);
        return;
    }
    if (mioSuit->isMuscleActive(false)){
        body->setPopStatus(partID, AbstractBodyPart::SecondEnabled);
        return;
    }
    body->setPopStatus(partID, AbstractBodyPart::BothDisabled);
}
