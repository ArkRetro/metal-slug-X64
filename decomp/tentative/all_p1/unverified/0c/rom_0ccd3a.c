/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ccd3a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_ccd3a(void)

{
  byte bVar1;
  undefined1 uVar3;
  undefined2 uVar2;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  bool bVar4;

  candidate_ce9b4();
  if ((bool)in_CF) {
    uVar3 = candidate_260c8();
    *(undefined1 *)((int)unaff_A6 + 0x72) = uVar3;
    uVar2 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x26) = uVar2;
  }
  else {
    uVar3 = candidate_260c8();
    *(undefined1 *)((int)unaff_A6 + 0x72) = uVar3;
    uVar2 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x26) = uVar2;
  }
  bVar1 = *(byte *)((int)unaff_A6 + 0x72) & 0x7f;
  *(byte *)((int)unaff_A6 + 0x72) = bVar1;
  if (bVar1 == 0) {
    *(undefined1 *)((int)unaff_A6 + 0x72) = 1;
  }
  *(undefined1 *)(unaff_A6 + 0x20) = 3;
  *unaff_A6 = 0xccda2;
  candidate_ce448();
  candidate_ce420();
  *(short *)(unaff_A6 + 0x26) = *(short *)(unaff_A6 + 0x26) + -1;
  if (*(short *)(unaff_A6 + 0x26) < 1) {
    uVar2 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x26) = uVar2;
    bVar4 = false;
    candidate_ce9b4();
    if (bVar4) {
      uVar2 = candidate_260c8();
      *(undefined2 *)(unaff_A6 + 0x26) = uVar2;
    }
    candidate_ce77c();
    *(char *)((int)unaff_A6 + 0x72) = *(char *)((int)unaff_A6 + 0x72) + -1;
    if (*(char *)((int)unaff_A6 + 0x72) < '\x01') {
      *unaff_A6 = candidate_cce0c;
    }
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_cce0c;
  }
  candidate_ce5da();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
    *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
    *(undefined1 *)((int)unaff_A6 + 0x73) = 2;
  }
  return;
}
