/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09f432.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9f432(void)

{
  undefined2 uVar1;
  ushort uVar2;
  int unaff_A6;

  if ((*(byte *)(unaff_A6 + 0x98) & 0x80) == 0) {
    uVar1 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x82) = uVar1;
    uVar1 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x84) = uVar1;
    uVar2 = candidate_29c6();
    *(short *)(unaff_A6 + 0x82) = ((uVar2 & 0x1c) - 0xe) + *(short *)(unaff_A6 + 0x82);
  }
  else {
    *(undefined2 *)(unaff_A6 + 0x82) = 0x20;
    *(undefined2 *)(unaff_A6 + 0x84) = 0x20;
    uVar2 = candidate_29c6();
    *(short *)(unaff_A6 + 0x82) = ((uVar2 & 8) - 4) + *(short *)(unaff_A6 + 0x82);
  }
  return;
}
