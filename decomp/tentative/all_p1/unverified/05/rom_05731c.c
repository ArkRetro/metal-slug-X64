/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05731c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5731c(void)

{
  int iVar1;
  undefined2 uVar2;
  short sVar3;
  code *extraout_A1;
  code *pcVar4;
  int unaff_A5;
  undefined4 *unaff_A6;

  if ((&DAT_0010fdb6)[(short)(*(ushort *)(unaff_A6 + 0x1d) & 1)] == '\0') {
    *(char *)(unaff_A6[3] + 0x60) = *(char *)(unaff_A6[3] + 0x60) + '\x01';
    candidate_4126();
    iVar1 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar4 = (code *)*unaff_A6;
      *(undefined4 *)(iVar1 + -4) = 0x424a;
      (*pcVar4)();
    } while( true );
  }
  uVar2 = *(undefined2 *)(unaff_A6[3] + 0x7a);
  if (*(short *)(unaff_A6 + 0x1d) != 0) {
    uVar2 = *(undefined2 *)(unaff_A6[3] + 0x7c);
  }
  *(undefined2 *)(unaff_A6 + 0x1e) = uVar2;
  candidate_572aa();
  *(undefined2 *)((int)unaff_A6 + 0x72) = 0xf;
  *unaff_A6 = 0x5735c;
  sVar3 = *(short *)((int)unaff_A6 + 0x72) + -1;
  *(short *)((int)unaff_A6 + 0x72) = sVar3;
  if (sVar3 == 0) {
    candidate_5728a();
    *(undefined2 *)((int)unaff_A6 + 0x72) = 8;
    *unaff_A6 = 0x57376;
    sVar3 = *(short *)((int)unaff_A6 + 0x72) + -1;
    *(short *)((int)unaff_A6 + 0x72) = sVar3;
    if (sVar3 == 0) {
      *(undefined2 *)((int)unaff_A6 + 0x72) = 0x1e;
      *unaff_A6 = 0x5738c;
      sVar3 = *(short *)((int)unaff_A6 + 0x72) + -1;
      *(short *)((int)unaff_A6 + 0x72) = sVar3;
      if (sVar3 == 0) {
        *unaff_A6 = candidate_573f0;
        candidate_c8c7e();
      }
      else {
        candidate_29f2();
      }
      uVar2 = candidate_5777e();
      *(undefined2 *)((int)unaff_A6 + 0x6e) = uVar2;
      pcVar4 = candidate_3dd96;
      sVar3 = 3;
      do {
        (*pcVar4)();
        sVar3 = sVar3 + -1;
        pcVar4 = extraout_A1;
      } while (sVar3 != 0);
      return;
    }
    return;
  }
  return;
}
