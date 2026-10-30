/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0515ce.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_515ce(void)

{
  code *pcVar1;
  int iVar2;
  byte bVar3;
  int unaff_A5;
  undefined4 *unaff_A6;

  candidate_1ad6e();
  candidate_50fcc();
  candidate_50fb4();
  *(undefined1 *)((int)unaff_A6 + 0x9f) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x8e) = 0xffff;
  *unaff_A6 = 0x515f8;
  if (*(char *)(unaff_A6[3] + 0x9b) == '\0') {
    if (*(short *)(unaff_A6[3] + 0x8e) != *(short *)((int)unaff_A6 + 0x8e)) {
      *(short *)((int)unaff_A6 + 0x8e) = *(short *)(unaff_A6[3] + 0x8e);
      candidate_4256();
    }
    if (*(byte *)(unaff_A6[3] + 0x9f) != *(byte *)((int)unaff_A6 + 0x9f)) {
      bVar3 = *(byte *)(unaff_A6[3] + 0x9f) & 0xf;
      if (bVar3 == 4) {
        candidate_510d4();
      }
      if (bVar3 == 2) {
        candidate_51082();
      }
      if (bVar3 == 1) {
        candidate_51032();
      }
    }
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)(unaff_A6[3] + 0x14);
    candidate_1abc8();
    candidate_427a();
    return;
  }
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
