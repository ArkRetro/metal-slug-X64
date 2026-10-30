/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0deb1a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_deb1a(void)

{
  code *pcVar1;
  char cVar2;
  int extraout_A0;
  int extraout_A0_00;
  int iVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar4;

  unaff_A6[0xc] = (*(int *)((int)unaff_A6 + 0x72) - unaff_A6[0xc] >> 1) + unaff_A6[0xc];
  unaff_A6[0xd] = (*(int *)((int)unaff_A6 + 0x76) - unaff_A6[0xd] >> 1) + unaff_A6[0xd];
  func_0x000143e4();
  candidate_427a();
  cVar2 = candidate_49e8();
  if (cVar2 == '\0') {
    cVar2 = '\0';
    if (*(char *)((int)unaff_A6 + 0x7e) == '\0') {
      candidate_3c5cc();
      *(char *)((int)unaff_A6 + 0x7e) = -cVar2;
    }
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x80) != 0) {
      *unaff_A6 = 0xdebce;
      cVar2 = '\0';
    }
    candidate_19504();
    if (!(bool)cVar2) {
      candidate_505a();
      return;
    }
  }
  else {
    candidate_120f8();
    bVar4 = false;
    candidate_40bc();
    iVar3 = extraout_A0;
    if (bVar4) {
      candidate_1b220();
      iVar3 = extraout_A0_00;
    }
    *(undefined1 *)(iVar3 + 0x5e) = *(undefined1 *)((int)unaff_A6 + 0x5e);
  }
  candidate_c0dfc();
  candidate_4126();
  iVar3 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar3 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
