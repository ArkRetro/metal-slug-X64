/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x030548.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_30548(void)

{
  undefined2 uVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  int unaff_A6;

  if (*(short *)(unaff_A6 + 0x80) == 0) {
    uVar1 = 4;
  }
  else {
    uVar1 = 0x15;
  }
  *(undefined2 *)(unaff_A6 + 0x28) = uVar1;
  *(undefined2 *)(unaff_A6 + 0x2c) = 0x14;
  sVar2 = *(short *)(unaff_A6 + 0x8a);
  sVar4 = 0;
  uVar1 = *(undefined2 *)(unaff_A6 + 0x28);
  do {
    sVar3 = *(short *)(unaff_A6 + 0x88) + sVar4;
    if (6 < sVar3) {
      sVar3 = sVar3 + -7;
    }
    candidate_30162(10 - (ushort)*(byte *)(unaff_A6 + sVar3 + 0x91),
                    *(int *)(unaff_A6 + 0x6e) + 0x6e + (int)(short)(sVar3 << 3));
    if (*(char *)(unaff_A6 + 0x90) == '\0') {
      *(short *)(unaff_A6 + 0x28) = *(short *)(unaff_A6 + 0x28) + 0xb;
    }
    else {
      *(short *)(unaff_A6 + 0x28) = *(short *)(unaff_A6 + 0x28) + 4;
    }
    candidate_30162();
    *(undefined2 *)(unaff_A6 + 0x28) = uVar1;
    *(short *)(unaff_A6 + 0x2c) = *(short *)(unaff_A6 + 0x2c) + 1;
    sVar4 = sVar4 + 1;
    sVar2 = sVar2 + -1;
  } while (0 < sVar2);
  return;
}
