/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x035e0a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_35e0a(void)

{
  code *pcVar1;
  int iVar2;
  char cVar3;
  short sVar4;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  undefined1 uVar5;

  candidate_1b2be();
  if ((bool)in_CF) {
    candidate_9ea8();
  }
  else {
    candidate_9ea8();
  }
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  cVar3 = candidate_29c6();
  sVar4 = 0xc;
  if (cVar3 < '\0') {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
    sVar4 = -0xc;
  }
  iVar2 = *(int *)(unaff_A6[3] + 0x82);
  unaff_A6[10] = *(undefined4 *)(iVar2 + 0x28);
  unaff_A6[0xb] = *(undefined4 *)(iVar2 + 0x2c);
  *(short *)(unaff_A6 + 10) = sVar4 + *(short *)(unaff_A6 + 10);
  *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + 0x10;
  candidate_4256();
  *unaff_A6 = 0x35e86;
  uVar5 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar5) {
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
