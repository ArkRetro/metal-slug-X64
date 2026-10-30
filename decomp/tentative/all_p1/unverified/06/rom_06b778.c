/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06b778.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6b778(void)

{
  undefined1 uVar1;
  undefined1 uVar2;
  char cVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  int unaff_A6;

  if ((*(byte *)(unaff_A6 + 0x15) & 0x40) == 0) {
    candidate_143dc();
  }
  else {
    uVar4 = *(undefined4 *)(unaff_A6 + 0x28);
    uVar5 = *(undefined4 *)(unaff_A6 + 0x2c);
    uVar6 = *(undefined4 *)(unaff_A6 + 0x30);
    uVar7 = *(undefined4 *)(unaff_A6 + 0x34);
    uVar8 = *(undefined4 *)(unaff_A6 + 0x38);
    uVar1 = *(undefined1 *)(unaff_A6 + 0x15);
    uVar2 = *(undefined1 *)(unaff_A6 + 0x5f);
    cVar3 = *(char *)(unaff_A6 + 0x6c);
    candidate_1456a();
    if (cVar3 != *(char *)(unaff_A6 + 0x6c)) {
      *(undefined4 *)(unaff_A6 + 0x28) = uVar4;
      *(undefined4 *)(unaff_A6 + 0x2c) = uVar5;
      *(undefined4 *)(unaff_A6 + 0x30) = uVar6;
      *(undefined4 *)(unaff_A6 + 0x34) = uVar7;
      *(undefined4 *)(unaff_A6 + 0x38) = uVar8;
      *(char *)(unaff_A6 + 0x6c) = cVar3;
      *(undefined1 *)(unaff_A6 + 0x5f) = uVar2;
      *(undefined1 *)(unaff_A6 + 0x15) = uVar1;
      candidate_1abc8();
      return;
    }
  }
  return;
}
