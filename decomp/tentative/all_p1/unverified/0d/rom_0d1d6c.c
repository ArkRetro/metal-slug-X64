/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d1d6c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d1d6c(void)

{
  uint uVar1;
  uint uVar2;
  undefined2 uVar3;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  candidate_d34ce();
  uVar3 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x8a) = uVar3;
  unaff_A6[0xc] = 0;
  if ((*(byte *)((int)unaff_A6 + 0x72) & 1) == 0) {
    *(undefined2 *)((int)unaff_A6 + 0x86) = 0x20;
    candidate_4256();
    *unaff_A6 = 0xd1e02;
    unaff_A6[0xc] = ((int)((uint)*(ushort *)((int)unaff_A6 + 0x86) << 0x10) >> 8) + unaff_A6[0xc];
    uVar2 = (int)((uint)*(ushort *)((int)unaff_A6 + 0x8a) << 0x10) >> 8;
    uVar1 = unaff_A6[0xc];
    if ((int)uVar2 >= (int)uVar1) {
      *unaff_A6 = candidate_d1e2c;
    }
    uVar4 = (int)uVar2 < (int)uVar1 && uVar2 < uVar1;
  }
  else {
    *(short *)((int)unaff_A6 + 0x8a) = -*(short *)((int)unaff_A6 + 0x8a);
    *(undefined2 *)((int)unaff_A6 + 0x86) = 0xffe0;
    candidate_4256();
    *unaff_A6 = 0xd1dbc;
    unaff_A6[0xc] = ((int)((uint)*(ushort *)((int)unaff_A6 + 0x86) << 0x10) >> 8) + unaff_A6[0xc];
    uVar2 = (int)((uint)*(ushort *)((int)unaff_A6 + 0x8a) << 0x10) >> 8;
    uVar1 = unaff_A6[0xc];
    if ((int)uVar2 >= (int)uVar1) {
      *unaff_A6 = candidate_d1e2c;
    }
    uVar4 = (int)uVar2 < (int)uVar1 && uVar2 < uVar1;
  }
  candidate_d3314();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_d1e70;
    uVar4 = 0;
  }
  func_0x000143e4();
  candidate_d3256();
  candidate_d31be();
  candidate_427a();
  candidate_d338a();
  candidate_d3280();
  candidate_4c24();
  if ((bool)uVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_d1f2e;
  }
  return;
}
