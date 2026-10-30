/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d8904.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d8904(void)

{
  undefined2 uVar1;
  short sVar2;
  undefined4 *unaff_A6;
  bool bVar3;

  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  if (*(char *)(unaff_A6 + 0x23) == '\0') {
    uVar1 = candidate_260c8();
  }
  else {
    uVar1 = candidate_260c8();
  }
  *(undefined2 *)(unaff_A6 + 0x2a) = uVar1;
  candidate_d939a();
  *(undefined4 *)((int)unaff_A6 + 0x72) = 0xe98ce;
  unaff_A6[0x14] = 0xe988a;
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 0x32;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  candidate_9ea8();
  candidate_4256();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  *unaff_A6 = 0xd89b8;
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_d8a1e;
  }
  candidate_427a();
  if (*(char *)((int)unaff_A6 + 0x83) == '\0') {
    sVar2 = candidate_49e8();
    if (sVar2 != 0) {
      *unaff_A6 = 0xd8a32;
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *unaff_A6 = candidate_d8a1e;
    }
  }
  bVar3 = false;
  candidate_19504();
  if (bVar3) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
