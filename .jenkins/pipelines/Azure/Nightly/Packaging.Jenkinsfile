// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

OECI_LIB_VERSION = env.OECI_LIB_VERSION ?: "master"
oe = library("OpenEnclaveCommon@${OECI_LIB_VERSION}").jenkins.common.Openenclave.new()

GLOBAL_TIMEOUT_MINUTES = 240
CTEST_TIMEOUT_SECONDS = 480
GLOBAL_ERROR = null

def LinuxPackaging(String version, String build_type, String lvi_mitigation = 'None') {
    stage("Ubuntu${version} SGX1FLC Package ${build_type} LVI_MITIGATION=${lvi_mitigation}") {
        node("ACC-${version}") {
            timeout(GLOBAL_TIMEOUT_MINUTES) {
                cleanWs()
                checkout scm
                def task = """
                           cmake ${WORKSPACE}                               \
                             -DCMAKE_BUILD_TYPE=${build_type}               \
                             -DCMAKE_INSTALL_PREFIX:PATH='/opt/openenclave' \
                             -DCPACK_GENERATOR=DEB                          \
                             -DLVI_MITIGATION=${lvi_mitigation}             \
                             -DLVI_MITIGATION_BINDIR=/usr/local/lvi-mitigation/bin
                           make
                           ctest --output-on-failure --timeout ${CTEST_TIMEOUT_SECONDS}
                           cpack -D CPACK_DEB_COMPONENT_INSTALL=ON -DCPACK_COMPONENTS_ALL=OEHOSTVERIFY
                           cpack
                           """
                oe.Run("clang-7", task)
                azureUpload(storageCredentialId: 'oe_jenkins_storage_account', filesPath: 'build/*.deb', storageType: 'blobstorage', virtualPath: "master/${BUILD_NUMBER}/ubuntu/${version}/${build_type}/lvi-mitigation-${lvi_mitigation}/SGX1FLC/", containerName: 'oejenkins')
                azureUpload(storageCredentialId: 'oe_jenkins_storage_account', filesPath: 'build/*.deb', storageType: 'blobstorage', virtualPath: "master/latest/ubuntu/${version}/${build_type}/lvi-mitigation-${lvi_mitigation}/SGX1FLC/", containerName: 'oejenkins')
            }
        }
    }
}

def WindowsPackaging(String label, String build_type, String has_quote_provider = 'OFF', String lvi_mitigation = 'None', String OE_SIMULATION = "0", String lvi_mitigation_skip_tests = 'OFF') {
    stage("Windows ${label} ${build_type} with SGX ${has_quote_provider} LVI_MITIGATION=${lvi_mitigation}") {
        node("SGXFLC-Windows-${label}-DCAP") {
            withEnv(["OE_SIMULATION=${OE_SIMULATION}"]) {
                timeout(GLOBAL_TIMEOUT_MINUTES) {
                oe.WinCompilePackageTest("build/X64-${build_type}", build_type, has_quote_provider, CTEST_TIMEOUT_SECONDS, lvi_mitigation, lvi_mitigation_skip_tests)
                azureUpload(storageCredentialId: 'oe_jenkins_storage_account', filesPath: 'build/*.nupkg', storageType: 'blobstorage', virtualPath: "v0.9.x/${BUILD_NUMBER}/windows/${label}/${build_type}/lvi-mitigation-${lvi_mitigation}/SGX1FLC/", containerName: 'oejenkins')
                azureUpload(storageCredentialId: 'oe_jenkins_storage_account', filesPath: 'build/*.nupkg', storageType: 'blobstorage', virtualPath: "v0.9.x/latest/windows/${label}/${build_type}/lvi-mitigation-${lvi_mitigation}/SGX1FLC/", containerName: 'oejenkins')
                }
            }
        }
    }
}

try{
    oe.emailJobStatus('STARTED')
    parallel "1604 SGX1FLC Package Debug" :                         { LinuxPackaging('1604', 'Debug') },
         "1604 SGX1FLC Package Debug LVI" :                         { LinuxPackaging('1604', 'Debug', 'ControlFlow') },
         "1604 SGX1FLC Package Release" :                           { LinuxPackaging('1604', 'Release') },
         "1604 SGX1FLC Package Release LVI" :                       { LinuxPackaging('1604', 'Release', 'ControlFlow') },
         "1604 SGX1FLC Package RelWithDebInfo" :                    { LinuxPackaging('1604', 'RelWithDebInfo') },
         "1604 SGX1FLC Package RelWithDebInfo LVI" :                { LinuxPackaging('1604', 'RelWithDebInfo', 'ControlFlow') },
         "1804 SGX1FLC Package Debug" :                             { LinuxPackaging('1804', 'Debug') },
         "1804 SGX1FLC Package Debug LVI" :                         { LinuxPackaging('1804', 'Debug', 'ControlFlow') },
         "1804 SGX1FLC Package Release" :                           { LinuxPackaging('1804', 'Release') },
         "1804 SGX1FLC Package Release LVI" :                       { LinuxPackaging('1804', 'Release', 'ControlFlow') },
         "1804 SGX1FLC Package RelWithDebInfo" :                    { LinuxPackaging('1804', 'RelWithDebInfo') },
         "1804 SGX1FLC Package RelWithDebInfo LVI" :                { LinuxPackaging('1804', 'RelWithDebInfo', 'ControlFlow') },
         "Win2016 Debug Cross Compile DCAP LVI" :                   { WindowsPackaging('2016', 'Debug', 'ON', 'ControlFlow', '0', 'ON') },
         "Win2016 Release Cross Compile DCAP LVI" :                 { WindowsPackaging('2016', 'Release', 'ON', 'ControlFlow', '0', 'ON') },
         "Win2019 Debug Cross Compile DCAP LVI" :                   { WindowsPackaging('2019', 'Debug', 'ON', 'ControlFlow', '0', 'ON') },
         "Win2019 Release Cross Compile DCAP LVI" :                 { WindowsPackaging('2019', 'Release', 'ON', 'ControlFlow', '0', 'ON') }
} catch(Exception e) {
    println "Caught global pipeline exception :" + e
    GLOBAL_ERROR = e
    throw e
} finally {
    currentBuild.result = (GLOBAL_ERROR != null) ? 'FAILURE' : "SUCCESS"
    oe.emailJobStatus(currentBuild.result)
}
