/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d7f2c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d7f2c(void)

{
  code *pcVar1;
  int iVar2;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar3;

  candidate_18f40();
  *(undefined2 *)(unaff_A6 + 0x26) = *(undefined2 *)(unaff_A6 + 0xf);
  *(undefined2 *)(unaff_A6 + 0xf) = 0x120;
  candidate_40bc();
  *(undefined1 *)(extraout_A0 + 0x82) = 5;
  bVar3 = false;
  candidate_40bc();
  if (bVar3) {
    candidate_1b220();
  }
  candidate_1db3e();
  *(undefined2 *)(unaff_A6 + 0xf) = *(undefined2 *)(unaff_A6 + 0x26);
  *(undefined1 *)(unaff_A6 + 0x21) = 0x8a;
  *(undefined1 *)((int)unaff_A6 + 0x85) = 0x8a;
  *(undefined1 *)((int)unaff_A6 + 0x86) = 0x8a;
  *(undefined1 *)((int)unaff_A6 + 0x87) = 0x8a;
  *(undefined1 *)(unaff_A6 + 0x22) = 0x8a;
  *(undefined1 *)((int)unaff_A6 + 0x89) = 0x8a;
  *(undefined1 *)((int)unaff_A6 + 0x8a) = 0x8a;
  *(undefined1 *)((int)unaff_A6 + 0x8b) = 0x8a;
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
