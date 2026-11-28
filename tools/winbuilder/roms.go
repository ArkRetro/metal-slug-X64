package main

// MAME 0.264 dumps (CRC-32 and SHA-1 as published in MAME's driver source).
var required = []romFile{
	{"mslugx", "250-c1.c1", 8388608, 0x09a52c6f, "c3e8a8ccdac0f8bddc4c3413277626532405fae2"},
	{"mslugx", "250-c2.c2", 8388608, 0x31679821, "554f600a3aa09c16c13c625299b087a79d0d15c5"},
	{"mslugx", "250-c3.c3", 8388608, 0xfd602019, "c56646c62387bc1439d46610258c755beb8d7dd8"},
	{"mslugx", "250-c4.c4", 8388608, 0x31354513, "31be8ea2498001f68ce4b06b8b90acbf2dcab6af"},
	{"mslugx", "250-c5.c5", 8388608, 0xa4b56124, "d41069856df990a1a99d39fb263c8303389d5475"},
	{"mslugx", "250-c6.c6", 8388608, 0x83e3e69d, "39be66287696829d243fb71b3fb8b7dc2bc3298f"},
	{"mslugx", "250-m1.m1", 131072, 0xfd42a842, "55769bad4860f64ef53a333e0da9e073db483d6a"},
	{"mslugx", "250-p1.p1", 1048576, 0x81f1f60b, "4c19f2e9824e606178ac1c9d4b0516fbaa625035"},
	{"mslugx", "250-p2.ep1", 4194304, 0x1fda2e12, "18aaa7a3ba8da99f78c430e9be69ccde04bc04d9"},
	{"mslugx", "250-s1.s1", 131072, 0xfb6f441d, "2cc392ecde5d5afb28ddbaa1030552b48571dcfb"},
	{"mslugx", "250-v1.v1", 4194304, 0xc79ede73, "ebfcc67204ff9677cf7972fd5b6b7faabf07280c"},
	{"mslugx", "250-v2.v2", 4194304, 0xea9aabe1, "526c42ca9a388f7435569400e2f132e2724c71ff"},
	{"mslugx", "250-v3.v3", 2097152, 0x2ca65102, "45979d1edb1fc774a415d9386f98d7cb252a2043"},
	{"neogeo", "000-lo.lo", 131072, 0x5a86cff2, "5992277debadeb64d1c1c64b0a92d9293eaf7e4a"},
	{"neogeo", "sfix.sfix", 131072, 0xc2ea0cfd, "fd4a618cdcdbf849374f0a50dd8efe9dbab706c3"},
	{"neogeo", "sm1.sm1", 131072, 0x94416d67, "42f9d7ddd6c0931fd64226a60dc73602b2819dcf"},
	{"neogeo", "sp-s2.sp1", 131072, 0x9036d879, "4f5ed7105b7128794654ce82b51723e16e389543"},
}
