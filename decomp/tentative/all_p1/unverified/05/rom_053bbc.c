/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x053bbc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_53bbc(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  undefined1 uVar4;

  candidate_52b5e();
  if ((bool)in_CF) {
    *unaff_A6 = 0x53c18;
  }
  sVar3 = *(short *)((int)unaff_A6 + 0x7a) + -1;
  *(short *)((int)unaff_A6 + 0x7a) = sVar3;
  if (sVar3 < 0) {
    sVar3 = candidate_49e8();
    if (sVar3 != 0) {
      *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ed;
      *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ed;
      candidate_120f8();
      unaff_A6[0x14] = 0;
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
      candidate_9f88();
      candidate_9ea8();
      candidate_4256();
      candidate_49e8();
      candidate_49e8();
      *unaff_A6 = candidate_53c80;
      return;
    }
  }
  *(undefined1 *)(unaff_A6 + 5) =
       *(undefined1 *)((int)unaff_A6 + (int)*(short *)((short)((_DAT_0010000e & 3) << 1) + 0x53b30))
  ;
  uVar4 = 0;
  candidate_427a();
  candidate_19504();
  if (!(bool)uVar4) {
    candidate_505a();
    return;
  }
  candidate_4192();
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
