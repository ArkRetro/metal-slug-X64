/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06e040.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6e040(void)

{
  code *pcVar1;
  int iVar2;
  byte bVar3;
  char cVar4;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar5;

  *unaff_A6 = 0x6e046;
  bVar5 = false;
  candidate_409a();
  if (!bVar5) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  cVar4 = candidate_aab84();
  if (cVar4 < '\0') {
    *unaff_A6 = 0x6e01c;
    return;
  }
  candidate_1aca2();
  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(extraout_A0 + 0x1e);
  bVar3 = *(byte *)(extraout_A0 + 0x82);
  *(byte *)(extraout_A0 + 0x82) = bVar3 & 0xfe;
  if ((bVar3 & 1) != 0) {
    *(undefined1 *)(unaff_A6 + 0x20) = *(undefined1 *)(extraout_A0 + 0x80);
    *(undefined1 *)((int)unaff_A6 + 0x81) = *(undefined1 *)(extraout_A0 + 0x81);
    if (*(short *)(extraout_A0 + 0x74) != *(short *)(unaff_A6 + 0x1d)) {
      *(short *)(unaff_A6 + 0x1d) = *(short *)(extraout_A0 + 0x74);
      candidate_9f88();
      candidate_9ea8();
    }
    candidate_4256();
  }
  candidate_1abc8();
  candidate_427a();
  return;
}
