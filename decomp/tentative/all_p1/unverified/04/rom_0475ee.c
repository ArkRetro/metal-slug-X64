/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0475ee.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_475ee(void)

{
  short sVar1;
  undefined2 uVar2;
  undefined4 extraout_A0;
  undefined4 *unaff_A6;
  bool bVar3;

  uVar2 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x21) = uVar2;
  candidate_1b04a();
  unaff_A6[0x1f] = extraout_A0;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  *(undefined1 *)((int)unaff_A6 + 0x9b) = 0;
  *(undefined1 *)(unaff_A6 + 0x2a) = 0;
  *(undefined2 *)(unaff_A6 + 0x24) = 0;
  candidate_4256();
  *unaff_A6 = 0x47630;
  candidate_49e9e();
  candidate_4936c();
  candidate_49f9e();
  func_0x000143e4();
  candidate_49b20();
  candidate_427a();
  bVar3 = false;
  candidate_49e74();
  if (bVar3) {
    *unaff_A6 = candidate_4768e;
  }
  if (*(short *)(unaff_A6 + 0x21) == 0) {
    *unaff_A6 = candidate_4768e;
    bVar3 = false;
  }
  else {
    sVar1 = *(short *)(unaff_A6 + 0x21);
    bVar3 = sVar1 == 0;
    *(short *)(unaff_A6 + 0x21) = sVar1 + -1;
  }
  candidate_49ece();
  if (bVar3) {
    *unaff_A6 = candidate_4783a;
  }
  *(char *)((int)unaff_A6 + 0x96) = *(char *)((int)unaff_A6 + 0x96) + '\x03';
  candidate_4900c();
  return;
}
