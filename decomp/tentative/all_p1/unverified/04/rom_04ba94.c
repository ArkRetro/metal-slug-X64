/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04ba94.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4ba94(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  short sVar3;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  *(undefined2 *)((int)unaff_A6 + 0x26) = 0x100;
  candidate_4256();
  if (*(char *)((int)unaff_A6 + 0x9e) == '\0') {
    *(undefined1 *)((int)unaff_A6 + 0x9a) = 2;
    uVar1 = unaff_A6[0xf];
    uVar2 = unaff_A6[0x10];
    sVar3 = *(short *)(unaff_A6[3] + 0x98);
    *(short *)(unaff_A6 + 0x26) = sVar3;
    sVar3 = sVar3 << 2;
    *(short *)(unaff_A6 + 0xf) = *(short *)(sVar3 + 0xe0f9a) + *(short *)(unaff_A6 + 0xf);
    *(short *)(unaff_A6 + 0x10) = *(short *)(sVar3 + 0xe0f9c) + *(short *)(unaff_A6 + 0x10);
    candidate_1b04a();
    sVar3 = candidate_1b12e();
    if (sVar3 < 0x81) {
      *(undefined1 *)(unaff_A6 + 0x27) = 8;
    }
    else {
      *(undefined1 *)(unaff_A6 + 0x27) = 0;
    }
    unaff_A6[0xf] = uVar2;
    unaff_A6[0x10] = uVar1;
  }
  else {
    *(undefined1 *)(unaff_A6 + 0x27) = 2;
  }
  *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
  *(undefined2 *)(unaff_A6 + 0x22) = 0;
  *unaff_A6 = 0x4bb2c;
  candidate_1abc8();
  if (*(short *)(unaff_A6 + 0x22) == 0) {
    *(undefined2 *)(unaff_A6 + 0x22) = 0;
    uVar4 = false;
    candidate_4d768();
    if ((bool)uVar4) {
      if (*(char *)((int)unaff_A6 + 0x9e) == '\0') {
        *unaff_A6 = candidate_4bb88;
        uVar4 = 0;
      }
      else {
        *unaff_A6 = candidate_4be74;
        uVar4 = 0;
      }
    }
  }
  else {
    sVar3 = *(short *)(unaff_A6 + 0x22);
    uVar4 = sVar3 == 0;
    *(short *)(unaff_A6 + 0x22) = sVar3 + -1;
  }
  candidate_4d9de();
  candidate_4d74a();
  if ((bool)uVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
    *(undefined1 *)((int)unaff_A6 + 0x9e) = 0xff;
    *(undefined1 *)(unaff_A6 + 0x27) = 2;
  }
  return;
}
