/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d691e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d691e(void)

{
  byte bVar1;
  undefined2 uVar2;
  undefined4 *unaff_A6;
  bool bVar3;
  undefined1 uVar4;

  if (*(char *)(unaff_A6[3] + 0x79) < '\x03') {
    uVar2 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x72) = uVar2;
  }
  else {
    uVar2 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x72) = uVar2;
  }
  candidate_4256();
  *unaff_A6 = 0xd697a;
  uVar4 = 0;
  candidate_d7160();
  candidate_44f6();
  candidate_427a();
  if ((bool)uVar4) {
    *unaff_A6 = 0xd687c;
    *(char *)((int)unaff_A6 + 0x76) = *(char *)((int)unaff_A6 + 0x76) + -1;
    if (*(char *)((int)unaff_A6 + 0x76) < '\x01') {
      *(undefined1 *)((int)unaff_A6 + 0x7d) = 0;
      uVar4 = candidate_260c8();
      *(undefined1 *)((int)unaff_A6 + 0xa3) = uVar4;
      uVar2 = candidate_260c8();
      *(undefined2 *)((int)unaff_A6 + 0x72) = uVar2;
      uVar4 = candidate_260c8();
      *(undefined1 *)((int)unaff_A6 + 0x76) = uVar4;
    }
  }
  bVar1 = *(byte *)(unaff_A6[3] + 0x79);
  if (bVar1 == 6) {
    *unaff_A6 = candidate_4188;
  }
  bVar3 = bVar1 < 6;
  candidate_409a();
  if (!bVar3) {
    *unaff_A6 = candidate_4188;
  }
  return;
}
