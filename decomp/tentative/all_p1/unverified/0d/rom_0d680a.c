/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d680a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d680a(void)

{
  char cVar1;
  byte bVar2;
  undefined2 uVar3;
  undefined1 uVar4;
  undefined4 *unaff_A6;
  bool bVar5;

  candidate_1ad6e();
  uVar3 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x72) = uVar3;
  uVar4 = candidate_260c8();
  *(undefined1 *)((int)unaff_A6 + 0x76) = uVar4;
  *(undefined1 *)((int)unaff_A6 + 0x7d) = 0;
  uVar4 = candidate_260c8();
  *(undefined1 *)((int)unaff_A6 + 0xa3) = uVar4;
  candidate_9ea8();
  candidate_4520();
  *(undefined4 *)((int)unaff_A6 + 0x92) = 0xe9250;
  candidate_4256();
  *unaff_A6 = 0xd6882;
  candidate_d7158();
  candidate_44f6();
  if ((*(byte *)(unaff_A6[3] + 0x5f) & 0x40) == 0) {
    cVar1 = *(char *)(unaff_A6[3] + 0xa1);
    *(undefined1 *)(unaff_A6[3] + 0xa1) = 0;
    if (cVar1 == '\0') {
      if (((*(char *)(unaff_A6[3] + 0x79) != '\x02') && (*(char *)(unaff_A6[3] + 0x79) != '\x03'))
         && (*(short *)((int)unaff_A6 + 0x72) = *(short *)((int)unaff_A6 + 0x72) + -1,
            *(short *)((int)unaff_A6 + 0x72) < 1)) {
        *unaff_A6 = candidate_d691e;
      }
    }
    else {
      *(undefined1 *)((int)unaff_A6 + 0x7d) = 0;
      uVar4 = candidate_260c8();
      *(undefined1 *)((int)unaff_A6 + 0xa3) = uVar4;
    }
  }
  candidate_427a();
  bVar2 = *(byte *)(unaff_A6[3] + 0x79);
  if (bVar2 == 6) {
    *unaff_A6 = candidate_4188;
  }
  bVar5 = bVar2 < 6;
  candidate_409a();
  if (!bVar5) {
    *unaff_A6 = candidate_4188;
  }
  return;
}
