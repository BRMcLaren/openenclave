@Library("OpenEnclaveCommon") _
oe = new jenkins.common.Openenclave()

OETOOLS_REPO = "https://oejenkinscidockerregistry.azurecr.io"
OETOOLS_REPO_CREDENTIAL_ID = "oejenkinscidockerregistry"
GLOBAL_TIMEOUT_MINUTES = 240

def buildDockerImages() {
    node("nonSGX") {
        timeout(GLOBAL_TIMEOUT_MINUTES) {
            stage("Checkout") {
                cleanWs()
                checkout scm
            }
            String buildArgs = oe.dockerBuildArgs("UID=\$(id -u)", "UNAME=\$(id -un)",
                                                  "GID=\$(id -g)", "GNAME=\$(id -gn)")

            stage("Build Ubuntu Deploy Docker image") {
                oeDeploy = oe.dockerImage(OE_DEPLOY_IMAGE, ".jenkins/Dockerfile.deploy", buildArgs)
            }
            stage("Push to OE Docker Registry") {
                docker.withRegistry(OETOOLS_REPO, OETOOLS_REPO_CREDENTIAL_ID) {
                    oeDeploy.push()
                }
            }
        }
    }
}


def buildVMImage(String os_type, String version, String imageName) {
    node("nonSGX") {
        stage("${os_type}-${version}") {
            timeout(GLOBAL_TIMEOUT_MINUTES) {
                cleanWs()
                checkout scm
                dir("${WORKSPACE}/.jenkins/provision/templates/packer/terraform") {
                    oe.azureEnvironment("""
                                        packer build -var-file=${os_type}-${version}-variables.json packer-${os_type}.json
                                        """, imageName)
                }
            }
        }
    }
}

buildDockerImages()
parallel "Build Ubuntu 16.04" : { buildVMImage("ubuntu", "16.04", OE_DEPLOY_IMAGE) },
         "Build Ubuntu 18.04" : { buildVMImage("ubuntu", "18.04", OE_DEPLOY_IMAGE) },
         "Build Ubuntu nonSGX" : { buildVMImage("ubuntu", "nonSGX", OE_DEPLOY_IMAGE) },
         "Build Windows 2016" : { buildVMImage("win", "2016", OE_DEPLOY_IMAGE) },
         "Build Windows 2016 DCAP" : { buildVMImage("win", "dcap", OE_DEPLOY_IMAGE) },
         "Build Windows 2016 nonSGX" : { buildVMImage("win", "nonSGX", OE_DEPLOY_IMAGE) }
