import { Component, OnInit, AfterViewInit, OnChanges, SimpleChanges, Input, ViewChild, ElementRef, HostListener } from '@angular/core';

@Component({
  selector: 'app-nonce-distribution',
  templateUrl: './nonce-distribution.component.html',
  styleUrls: ['./nonce-distribution.component.scss']
})
export class NonceDistributionComponent implements OnInit, AfterViewInit, OnChanges {

  @Input() nonceDistribution: number[] = [];
  @Input() asicCount: number = 4;
  @Input() scalingExponent: number = 0.5;

  private chipValues: number[] = [];

  @ViewChild('canvas', { static: true }) canvas!: ElementRef<HTMLCanvasElement>;
  private ctx!: CanvasRenderingContext2D;

  private chipMarkerRadius: number = 5;
  private scaledMarkerRadius: number = 8;

  tooltipText: string = '';
  tooltipVisible: boolean = false;
  tooltipX: number = 0;
  tooltipY: number = 0;

  constructor() {}

  ngOnInit(): void {
    this.updateChipValues();
  }

  ngAfterViewInit(): void {
    this.ctx = this.canvas.nativeElement.getContext('2d') as CanvasRenderingContext2D;
    this.drawBalanceVisualization();
  }

  ngOnChanges(changes: SimpleChanges): void {
    if (changes['nonceDistribution'] || changes['asicCount']) {
      this.updateChipValues();
      this.drawBalanceVisualization();
    }
    if (changes['scalingExponent']) {
      this.drawBalanceVisualization();
    }
  }

  private updateChipValues(): void {
    // Ensure the chip values array matches the ASIC count
    this.chipValues = [...this.nonceDistribution];

    // If the array is shorter than the ASIC count, pad it with default values
    while (this.chipValues.length < this.asicCount) {
      this.chipValues.push(0);
    }

    // Trim extra values if necessary
    this.chipValues = this.chipValues.slice(0, this.asicCount);
  }

  private getMedianValue(): number {
    const sortedValues = [...this.chipValues].sort((a, b) => a - b);
    const mid = Math.floor(sortedValues.length / 2);
    return sortedValues.length % 2 !== 0
      ? sortedValues[mid]
      : (sortedValues[mid - 1] + sortedValues[mid]) / 2;
  }

  private drawBalanceVisualization(): void {
    if (!this.ctx) {
      return;
    }

    const canvasElem = this.canvas.nativeElement;
    const width = canvasElem.width;
    const height = canvasElem.height;
    const centerX = width / 2;
    const centerY = height / 2;
    const margin = 20;
    const outerRadius = (Math.min(width, height) / 2) - margin;

    // Clear canvas before drawing
    this.ctx.clearRect(0, 0, width, height);

    // Fill the circle
    this.ctx.beginPath();
    this.ctx.arc(centerX, centerY, outerRadius, 0, 2 * Math.PI);
    this.ctx.fillStyle = '#304562';
    this.ctx.fill();

    // Radar rings
    this.ctx.strokeStyle = '#1f2d40';
    this.ctx.lineWidth = 1;
    for (let i = 1; i <= 3; i++) {
      this.ctx.beginPath();
      this.ctx.arc(centerX, centerY, (outerRadius / 3) * i, 0, 2 * Math.PI);
      this.ctx.stroke();
    }

    // Radar spokes
    for (let i = 0; i < this.asicCount; i++) {
      const angle = (2 * Math.PI / this.asicCount) * i;
      const xEnd = centerX + outerRadius * Math.cos(angle);
      const yEnd = centerY + outerRadius * Math.sin(angle);
      this.ctx.beginPath();
      this.ctx.moveTo(centerX, centerY);
      this.ctx.lineTo(xEnd, yEnd);
      this.ctx.stroke();
    }

    const medianValue = this.getMedianValue();
    const chipPositions: { x: number; y: number; value: number }[] = [];
    let faulty = false;
    let warning = false;

    for (let i = 0; i < this.asicCount; i++) {
        const angle = (2 * Math.PI / this.asicCount) * i - Math.PI / 2;
        const x = centerX + outerRadius * Math.cos(angle);
        const y = centerY + outerRadius * Math.sin(angle);
        const value = this.chipValues[i] || 0;
        chipPositions.push({ x, y, value });

        if (value === 0) {
          faulty = true;
        } else if (Math.abs((value - medianValue) / medianValue) >= 0.05) {
          warning = true;
        }
    }

    // Draw the outer circle border
    this.ctx.beginPath();
    this.ctx.arc(centerX, centerY, outerRadius, 0, 2 * Math.PI);
    this.ctx.strokeStyle = '#64b5f6';
    this.ctx.lineWidth = 4;
    this.ctx.stroke();

    for (let i = 0; i < this.asicCount; i++) {
      const angle = (2 * Math.PI / this.asicCount) * i - Math.PI / 2;
      const x = centerX + outerRadius * Math.cos(angle);
      const y = centerY + outerRadius * Math.sin(angle);

      // Draw the chip marker
      this.ctx.beginPath();
      this.ctx.arc(x, y, this.chipMarkerRadius, 0, 2 * Math.PI);
      this.ctx.fillStyle = '#3498db';
      this.ctx.fill();
      this.ctx.strokeStyle = '#2980b9';
      this.ctx.lineWidth = 1;
      this.ctx.stroke();
    }

    let totalWeight = 0;
    let weightedX = 0;
    let weightedY = 0;
    chipPositions.forEach(pos => {
      totalWeight += pos.value;
      weightedX += pos.x * pos.value;
      weightedY += pos.y * pos.value;
    });
    const computedCenterX = weightedX / totalWeight;
    const computedCenterY = weightedY / totalWeight;

    const dx = computedCenterX - centerX;
    const dy = computedCenterY - centerY;
    const r = Math.sqrt(dx * dx + dy * dy);

    let scaledCenterX = centerX;
    let scaledCenterY = centerY;
    if (r !== 0) {
      const scaledR = outerRadius * Math.pow(r / outerRadius, this.scalingExponent);
      scaledCenterX = centerX + (dx / r) * scaledR;
      scaledCenterY = centerY + (dy / r) * scaledR;
    }

    // Draw a dashed white line from geometric center to scaled center
    this.ctx.setLineDash([5, 5]);
    this.ctx.beginPath();
    this.ctx.moveTo(centerX, centerY);
    this.ctx.lineTo(scaledCenterX, scaledCenterY);
    this.ctx.strokeStyle = '#FFFFFF';
    this.ctx.lineWidth = 1;
    this.ctx.stroke();
    this.ctx.setLineDash([]);

    // Draw the scaled center marker in purple (unchanged size)
    this.ctx.beginPath();
    this.ctx.arc(scaledCenterX, scaledCenterY, this.scaledMarkerRadius, 0, 2 * Math.PI);
    this.ctx.fillStyle = faulty ? '#ff0000' : warning ? '#ffa500' : '#8e44ad';
    this.ctx.fill();
    this.ctx.strokeStyle = '#71368a';
    this.ctx.stroke();

  }

  @HostListener('mousemove', ['$event'])
  onMouseMove(event: MouseEvent): void {
    const rect = this.canvas.nativeElement.getBoundingClientRect();
    const x = event.clientX - rect.left;
    const y = event.clientY - rect.top;

    // Show tooltip on hover
    this.tooltipText = this.chipValues.join('/'); // Format as "123/126/200/100"
    this.tooltipX = x + 10;
    this.tooltipY = y - 10;
    this.tooltipVisible = true;
  }

  @HostListener('mouseleave')
  onMouseLeave(): void {
    this.tooltipVisible = false;
  }
}
