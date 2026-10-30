/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05764a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5764a(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;

  *(undefined2 *)(unaff_A6 + 10) = 4;
  *(undefined2 *)(unaff_A6 + 0xb) = 0xf;
  *unaff_A6 = 0x5765c;
  candidate_3d3b2();
  *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + -1;
  candidate_3d3b2();
  if (*(short *)(unaff_A6 + 0xb) == 5) {
    *(undefined1 *)(unaff_A6[3] + 0x60) = 0xff;
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  return;
}
